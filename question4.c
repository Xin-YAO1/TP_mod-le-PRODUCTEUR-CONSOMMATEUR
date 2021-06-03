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

/* Functions */

/*Produire()est une fonction qui génère un message « Message : k» ou k est incrémenté à chaque appel de cette fonction*/
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

    //stockez le message dans la liste de indice id
    T[id] = (char*)malloc(strlen(message)*sizeof(char));
    strcpy(T[id],message);
    printf("deposer: %s at T[%d]\n",message,id);
    
    //pour stockez le message suivant dans la liste de indice (id+1)%N
    //(id+1)%N car si on arrive a la fin de la liste, il nous faut revient au debut
    id=(id+1)%N;
}

char* retirer(){

    // prenez le message de la liste en indice ir
    char *mess =  (char*)malloc(strlen(T[ir])*sizeof(char));;
    strcpy(mess,T[ir]);
    printf("retirer: %s at T[%d]\n",mess,ir);
    
    // liberer le stockage
    free(T[ir]);
    ir=(ir+1)%N;
    return mess;
}

//juste un seul production() et consommation(), pas besoins de utiliser le boucle

void production(){
    //while(1){
    char* m=produire();
    //deposer d'abord
    sem_wait(&sprod);
    deposer(m);
    sem_post(&scon);
    sleep(1);
    //}
}

void consommation(){
    //while(1){
    sem_wait(&scon);
    char* r=retirer();
    // puis retirer
    sem_post(&sprod);
    consommer(r);
    free(r);
    sleep(1);
    //}
}


int main(){

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