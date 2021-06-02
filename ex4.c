#include <stdio.h>
#include <stdlib.h>
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

void main(){
    char* m=produire();
    printf("%s",m);
    //printf("%d",strlen(m));
    printf("\n");
    consommer(m);
    printf("\n");
    deposer(m);
    printf("\n");
    char* r=retirer();
    printf("%s",r);
    
}
