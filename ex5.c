
// C program for array implementation of queue
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>
#include <string.h>
#define N 10


/* Global variables */
int k = 0;
int id = 0;
int ir = 0;
char buff[20];
char* T[10];

/* Functions */
char* produire(){
    int l = k;
    k=k+1;
    sprintf(buff,"Message : %d \n",l);
    return(buff);
}

void consommer(char *message){
    int i;
    for (i=0;i<strlen(message);i++){
        
        if (message[i]>='a' && message[i]<='z'){
            
            message[i]=message[i]-32;
        }
    }
    printf("%s",message);
}

void deposer(char* message){
    T[id] = message;
    printf("%s",message);
    id=(id+1)%N;
}

char* retirer(){
    char* mess=T[ir];
    ir=(ir+1)%N;
    sprintf(buff,"Message : %d \n",mess);
    return(buff);
}




// A structure to represent a queue
struct Queue
{
    int front, rear, size;
    unsigned capacity;
    char* array;
};

// function to create a queue of given capacity.
// It initializes size of queue as 0
struct Queue* createQueue(unsigned capacity)
{
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;  // This is important, see the enqueue
    queue->array = (char*) malloc(queue->capacity * sizeof(int));
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
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
    printf("%d enqueued to queue\n", item);
}

// Function to remove an item from queue.
// It changes front and size
int dequeue(struct Queue* queue)
{
    if (isEmpty(queue))
        return ;
    char* item = queue->array[queue->front];
    queue->front = (queue->front + 1)%queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

// Function to get front of queue
int front(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->front];
}

// Function to get rear of queue
int rear(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->rear];
}

// Driver program to test above functions./
int main()
{
    struct Queue* queue = createQueue(1000);
    char* m=produire();
    
    consommer(m);
    
    enqueue(queue, m);
    
    deposer(m);
    enqueue(queue, m);
   
    char* r=retirer();
    enqueue(queue, r);
    

    printf("%d dequeued from queue\n\n", dequeue(queue));

    printf("Front item is %d\n", front(queue));
    printf("Rear item is %d\n", rear(queue));
    
   
   

    return 0;
}
 
