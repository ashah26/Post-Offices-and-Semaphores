//
// Created by ashna on 11/30/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>


// Declaring static global variables
static int no_of_free_clerks;
static int no_of_customers_arrived;
static int no_of_customers_waited;

//Variable to count total number of customers
int total_customers = 0;

//Intializing the thread condition variable
static pthread_cond_t ok = PTHREAD_COND_INITIALIZER;

//Declaring the thread mutex variable
static pthread_mutex_t lock ;

//Defining structure for reading the line from input file
struct times{
    int customer_id;
    int wait_time;
    int process_time;
};

void customer(void *arg){

    //assigning the argument in the structure for customer
    struct times *times1;
    times1 = (struct times *)arg;

    //Start the critical section
    pthread_mutex_lock(&lock);

    //Increase the customers arrived.
    no_of_customers_arrived = no_of_customers_arrived + 1;
    printf("Customer #'%d' arrives at the post office. \n", times1->customer_id);

    //Checking if clerk is free else increasing the waited customers and asking to wait
    while(no_of_free_clerks == 0){
        no_of_customers_waited = no_of_customers_waited + 1;
        pthread_cond_wait(&ok, &lock);
    }

    //If clerk is free customer will start getting help and we will decrease number of free clerks
    printf("Customer #'%d' starts getting helped. \n", times1->customer_id);
    no_of_free_clerks = no_of_free_clerks -1;

    //Release the lock
    pthread_mutex_unlock(&lock);

    //Perform sleep on the thread that got clerk for the time period of processing time
    sleep(times1->process_time);

    //Start the critical section again
    pthread_mutex_lock(&lock);

    //Process being over, customer leaves and increase count of free clerks by 1
    no_of_free_clerks = no_of_free_clerks + 1;
    printf("Customer #'%d' leaves the office \n", times1->customer_id);

    //Signal the first waiting thread in queue that clerk is free to attend customer
    pthread_cond_signal(&ok);

    //Release the lock
    pthread_mutex_unlock(&lock);
}



int main(int argc, char* argv[]){

    //Getting the number of free clerks from user
    printf("Number of clerks: %d \n", atoi(argv[1]));
    no_of_free_clerks = atoi(argv[1]);

    //Initializing thread mutex variable
    if(pthread_mutex_init(&lock, NULL) !=0){
        printf("Mutex init failed \n");
        return  -1;
    }


    //Declaring an array of structure for all the customers
    struct times times1[500];

    //Declaring local variables to store data
    int customerId, waitTime, processTime;

    //Declaring thread object to pass
    pthread_t threadId;

    //Declaring thread array to keep a track of all customers
    pthread_t customer_ids[500];

    //Read the file
    while(scanf("%d %d %d", &customerId, &waitTime, &processTime)>0){

        total_customers += 1;

        //Adding to the structure
        times1[total_customers-1].customer_id = customerId;
        times1[total_customers-1].process_time = processTime;
        times1[total_customers-1].wait_time = waitTime;

        //Checking the wait time and creating the thread accordingly
        if(waitTime == 0){
            pthread_create(&threadId, NULL, customer, &times1[total_customers-1]);
        }else{
            sleep(waitTime);
            pthread_create(&threadId, NULL, customer, &times1[total_customers-1]);
        }

        //assigning threadId to the array of customers
        customer_ids[total_customers-1] = threadId;
    }

    // Looping to synchronize various threads
    int a;

    for(a=0; a<total_customers; a++){
        pthread_join(customer_ids[a], NULL);
    }

    //Summary of customers
    printf("'%d' customer(s) went serviced \n", total_customers);
    printf("'%d' customer(s) had to wait \n", no_of_customers_waited);
    printf("'%d' customer(s) did not have to wait \n", total_customers-no_of_customers_waited);


    return 0;
}


