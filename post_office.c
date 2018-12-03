//
// Created by ashna on 11/30/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>


static int no_of_free_clerks;
static int no_of_customers_arrived;
static int no_of_customers_waited;
static int no_of_customers_serviced;

int total_customers = 0;

static pthread_cond_t ok = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t lock ;

struct times{
    int customer_id;
    int wait_time;
    int process_time;
};

void customer(void *arg){

    struct times *times1;
    times1 = (struct times *)arg;

    pthread_mutex_lock(&lock);
    no_of_customers_arrived = no_of_customers_arrived + 1;
    printf("Customer #'%d' arrives at the post office. \n", times1->customer_id);
    while(no_of_free_clerks == 0){
        no_of_customers_waited = no_of_customers_waited + 1;
        pthread_cond_wait(&ok, &lock);
    }
    printf("Customer #'%d' starts getting helped. \n", times1->customer_id);
    no_of_free_clerks = no_of_free_clerks -1;
    pthread_mutex_unlock(&lock);

    sleep(times1->process_time);

    pthread_mutex_lock(&lock);
    no_of_free_clerks = no_of_free_clerks + 1;
    printf("Customer #'%d' leaves the office \n", times1->customer_id);
    no_of_customers_serviced = no_of_customers_serviced + 1;
    pthread_cond_signal(&ok);

//    pthread_cond_broadcast(&ok);
    pthread_mutex_unlock(&lock);
}



int main(int argc, char* argv[]){

    printf("Number of clerks: %d \n", atoi(argv[1]));
    no_of_free_clerks = atoi(argv[1]);

    if(pthread_mutex_init(&lock, NULL) !=0){
        printf("Mutex init failed \n");
        return  -1;
    }


    struct times times1[500];

    int customerId, waitTime, processTime;
    pthread_t threadId;
    pthread_t customer_ids[500];
    while(scanf("%d %d %d", &customerId, &waitTime, &processTime)>0){

        total_customers += 1;

        times1[total_customers-1].customer_id = customerId;
        times1[total_customers-1].process_time = processTime;
        times1[total_customers-1].wait_time = waitTime;

        if(waitTime == 0){
            pthread_create(&threadId, NULL, customer, &times1[total_customers-1]);
        }else{
            sleep(waitTime);
            pthread_create(&threadId, NULL, customer, &times1[total_customers-1]);
        }
        customer_ids[total_customers-1] = threadId;
    }

    int a;

    for(a=0; a<total_customers; a++){
        pthread_join(customer_ids[a], NULL);
    }
    printf("'%d' customer(s) went serviced \n", total_customers);
    printf("'%d' customer(s) had to wait \n", no_of_customers_waited);
    printf("'%d' customer(s) did not have to wait \n", total_customers-no_of_customers_waited);


    return 0;
}


