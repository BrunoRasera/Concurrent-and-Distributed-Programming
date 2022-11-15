/*
Programação Concorrente e Distribuída - 2022/2

Trabalho 2 - Atividade 01
Manna-Pnueli - PThreads

Bruno Rasera
Letícia Lisboa
Daniel Paiva

To increase the stack size in linux and avoid segmentation fault errors, use ulimit -s unlimited 
To compite pthread version, use  the -pthread flag on gcc
*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

#define NUM_THREADS 2

int NUM_EXECS = 1000000000; //Default
int NO_REST = 0;

int respond = 0;
int request = 0;
int SOMA = 0;

void criticalSection(int callerId){
    printf("\n>Thread %d entering CS: ", callerId);
    int local = SOMA;
    if (NO_REST == 0) sleep(rand()%2);
    SOMA = local + 1;
    printf("Current Sum: %d\n", SOMA);
    printf("\n<Thread %d leaving CS\n", callerId);
}

void *clientProcess(void *threadarg){
    int i = 0;
    int *th_id = (int*) threadarg; //thread id

    for (i = 0; i < NUM_EXECS; i++) //each thread executes the CS NUM_EXECS times
    {
        while (respond != *th_id) {request = *th_id;}
        //Critical Section:
        criticalSection(*th_id);

        //End of Critical Section
        respond = 0;
    }
    
    pthread_exit(NULL);
}

void *serverProcess(void *threadarg){
    int i = 0;
    int *th_id = (int*) threadarg; //thread id

    for (i = 0; i < NUM_EXECS*NUM_THREADS; i++) //since each thread executes the CS NUM_EXECS times
    {
        while (request == 0){if (request != 0) break;}
        respond = request;
        while (respond != 0){if (respond == 0) break;}
        request = 0;
    }
    
    pthread_exit(NULL);
}

void checkArgs(int argc, char *argv[]){
    if (argc > 2) {
        if (strcmp(argv[1], "noSleep")==0 || strcmp(argv[2], "noSleep")==0) NO_REST = 1;
        if (strcmp(argv[1], "test")==0 || strcmp(argv[2], "test")==0) NUM_EXECS = 10;
    } else if (argc > 1) {
        if (strcmp(argv[1], "noSleep")==0) NO_REST = 1;
        if (strcmp(argv[1], "test")==0) NUM_EXECS = 10;
    }
}

int main(int argc, char *argv[])
{
    checkArgs(argc, argv);
    int i = 0, thread_id[NUM_THREADS+1];
    pthread_t thread[NUM_THREADS+1];

    thread_id[0] = 0; //Server
    pthread_create(&thread[i], NULL, &serverProcess, (void *)&thread_id[0]);

    //Creating CLient Threads
    for (i = 1; i <= NUM_THREADS; i++) //since thread 0 is server...
    {
        thread_id[i] = i;
        pthread_create(&thread[i], NULL, &clientProcess, (void *)&thread_id[i]);
    }

    // Joining ALL threads
    for (i = 0; i <= NUM_THREADS; i++)
    {
        pthread_join(thread[i], NULL);
    }

    printf("\nFinal Sum: %d expected %d\n", SOMA, NUM_EXECS*NUM_THREADS);
    pthread_exit(NULL);
    return 0;
}
