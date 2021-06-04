// C program for array implementation of queue
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#define N 5
#define MESSAGE_SIZE 20


/* Global variables */
int k = 0;
int id = 0;
int ir = 0;
char* T[N];
char buff[MESSAGE_SIZE];

sem_t sprod, scon;

struct Queue* queue;



// A structure to represent a queue
struct Queue
{
    int front, rear, size;
    unsigned capacity;
    char** array;
};

// function to create a queue of given capacity.
// It initializes size of queue as 0
struct Queue* createQueue(unsigned capacity)
{
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = 0;
    queue->size = 0;
    queue->rear = capacity - 1;  // This is important, see the enqueue
    queue->array = (char**) malloc(queue->capacity * sizeof(char*));
    return queue;
}

// Queue is full when size becomes equal to the capacity
int isFull(struct Queue* queue)
{  return (queue->size == queue->capacity);  }

// Queue is empty when size is 0
int isEmpty(struct Queue* queue)
{  return (queue->size == 0); }

// Function to add an item to the queue.
// It changes rear and size
void enqueue(struct Queue* queue, char* item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)%queue->capacity;

    queue->array[queue->rear] = (char*) malloc(strlen(item) * sizeof(char));
    strcpy(queue->array[queue->rear],item);

    queue->size = queue->size + 1;
    printf("enqueued at [%d]: %s \n", queue->rear,item);
}

// Function to remove an item from queue.
// It changes front and size
char* dequeue(struct Queue* queue)
{
    if (isEmpty(queue))
        return "";
    char* current = queue->array[queue->front];
    char* item = (char*) malloc(strlen(current) * sizeof(char));
    strcpy(item,current);
    free(current);
    printf("dequeue [%d]: %s \n", queue->front, item);

    queue->front = (queue->front + 1)%queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

// I think we do not need these 2 functions
// Function to get front of queue
int front(struct Queue* queue)
{
    if (isEmpty(queue))
        return -1;
    return queue->front;
}

// Function to get rear of queue
int rear(struct Queue* queue)
{
    if (isEmpty(queue))
        return -1;
    return queue->rear;
}



/* Functions */



char* produire(){
    int l = k;
    k=k+1;
    //garder le message a char buff
    sprintf(buff,"Message : %d ",l);
    return buff;
}


/*Consommer() met le message reçu en majuscule*/
void consommer(char *message){
    int i;
    for (i=0;i<strlen(message);i++){
        
        if (message[i]>='a' && message[i]<='z'){
            
            message[i]=message[i]-32;
        }
    }
    printf("consommer: %s\n",message);
}

/*deposer() est pour deposer les message dans une liste de char*/
void deposer(char* message){
    enqueue(queue,message);
}

char* retirer(){
    char *mess =  dequeue(queue);
    return mess;
}

void production(){
    while(1){
    char* m=produire();
    //deposer d'abord
    sem_wait(&sprod);
    deposer(m);
    sem_post(&scon);
    sleep(1);
    }
}

void consommation(){
    while(1){
    sem_wait(&scon);
    char* r=retirer();
    // puis retirer
    sem_post(&sprod);
    consommer(r);
    free(r);
    sleep(1);
    }
}








// Driver program to test above functions./
int main()
{
    queue = createQueue(1000);
    

    pthread_t thread1, thread2;


    if (sem_init(&sprod, 0, 1) == -1) {
        perror("Could not initialize mylock semaphore sprod");
        exit(2);
    }
    if (sem_init(&scon, 0, 0) == -1) {
        perror("Could not initialize mylock semaphore scon");
        exit(2);
    }
    if (pthread_create(&thread1, NULL, production, NULL) < 0) {
        perror("Error: thread cannot be created");
        exit(1);
    }
    if (pthread_create(&thread2, NULL, consommation, NULL) < 0) {
        perror("Error: thread cannot be created");
        exit(1);
    }

     /* wait for created thread to terminate */
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

/* destroy semaphore sync */
    sem_destroy(&sprod);
    sem_destroy(&scon);

    exit(0);

    return 0;
}


