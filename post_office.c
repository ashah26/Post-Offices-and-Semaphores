//
// Created by ashna on 11/30/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


    void print_message_function(void *ptr);

     int main(int argc, char* argv[]){

         printf("Number of clerks: %d \n", atoi(argv[1]));

        FILE *fp;
        char c[1000];

        fp = fopen("./Sample.txt","r");

        if(fp == NULL){
            return -1;
        }else{

            while(fgets(c, sizeof(c),fp) != NULL){
                printf("%s", c);
            }
            fclose(fp);
        }



//         pthread_t thread1, thread2;
//         char *message1 = "This is First Thread";
//         char *message2 = "This is Second Thread";
//         int returnThread1, returnThread2;
//
//         //independent threads
//         returnThread1 = pthread_create(&thread1, NULL, print_message_function,(void *) message1);
//         returnThread2 = pthread_create(&thread2, NULL, print_message_function, (void *) message2);
//
//         pthread_join(thread1, NULL);
//         pthread_join(thread2, NULL);
//
//         printf("Thread 1 returns: %d \n",returnThread1);
//         printf("Thread 2 returns: %d \n",returnThread2);
//
         return 0;
//
//

    }

void print_message_function(void *ptr){
    char *message;
    message = (char *)ptr;
    printf("%s \n",message);
}


