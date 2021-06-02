 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
/* Global variables */

sem_t sync1;
sem_t sync2;
/* Thread function */

void a1(){
  printf("je suis a1 \n");
  
}

void a2(){
  printf("je suis a2 \n");
  
}

void b1(){
  printf("je suis b1 \n");
}

void b2(){
  printf("je suis b2 \n");
}

void *p1(void *arg)
{
    a1();
    sem_post(&sync1);
    
    sem_wait(&sync2);
    a2();

}



void *p2(void *arg)
{
    sem_wait(&sync1);
    b2();
    
    sem_post(&sync2);
    b1();
    
}




void main () {
    pthread_t thread1, thread2;
    /* semaphore sync should be initialized by 0 */
    if (sem_init(&sync1, 0, 0) == -1) {
        perror("Could not initialize mylock semaphore");
        exit(2);
    }
    if (pthread_create(&thread1, NULL, p1, NULL) < 0) {
        perror("Error: thread cannot be created");
        exit(1);
    }
    if (pthread_create(&thread2, NULL, p2, NULL) < 0) {
        perror("Error: thread cannot be created");
        exit(1);
    }
    /* wait for created thread to terminate */
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    /* destroy semaphore sync */
    sem_destroy(&sync1);
    exit(0);
}
