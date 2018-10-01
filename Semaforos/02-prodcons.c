#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 
#include "semaphores.h"

#define TAMBUFFER 5

#define VELPROD 6
#define VELCONS 6

void *productor(void *arg);
void *consumidor(void *arg);

char buffer[TAMBUFFER];
SEM_ID max,blok,exmut;

int main()
{
    // Definición de variables
    int res;
    int n;
    pthread_t a_thread[2];     // Identificador del hilo
    void *thread_result;

	srand(getpid());
	
    // Creación de semáforos
    max=createsem((key_t) 9234,TAMBUFFER);

    blok=createsem((key_t) 9235,0);

    exmut=createsem((key_t) 9236,1);

    printf("Semáforos creados\n");

    /* Aquí se crean los hilos */

    res = pthread_create(&(a_thread[0]), NULL, productor, NULL);
    if (res != 0) {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }

    res = pthread_create(&(a_thread[1]), NULL, consumidor, NULL);
    if (res != 0) {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }



    printf("Esperando que los hilos terminen...\n");
    for(n=0;n<=1;n++)
    {
        res=pthread_join(a_thread[n], &thread_result);
        if (res != 0)
        {
            perror("La unión del hilo ha fallado");
            exit(EXIT_FAILURE);
        }
    }


    // Borrar los semáforos

    erasesem(max);
    erasesem(blok);
    erasesem(exmut);

    exit(EXIT_SUCCESS);
}


void *productor(void *arg)
{
    int n;

    printf("Inicia productor\n");
    for(n=0;n<100;n++)
    {
        semwait(max);	// Si se llena el buffer se bloquea
        semwait(exmut);	// Asegurar el buffer como sección crítoca

        buffer[n%TAMBUFFER]=65+n%26;
        printf("Productor produce %c\n",(char) 65+n%26);
        sleep(rand()%VELPROD);

        semsignal(exmut);	// Libera la sección crítica del buffer
        semsignal(blok);	// Si el consumidor está bloqueado porque el buffer está vacío, lo desbloqueas

        sleep(rand()%VELPROD);

    }
    pthread_exit(NULL);
}


void *consumidor(void *arg)
{
    int n;

    printf("Inicia Consumidor\n");
    for(n=0;n<100;n++)
    {
        semwait(blok);	// Si el buffer está vacío, se bloquea
        semwait(exmut);	// Asegura el buffer como sección crítica 

        printf("                   Consumidor consume %c\n",(char) buffer[n%TAMBUFFER]);
        sleep(rand()%VELCONS);

        semsignal(exmut);	// Libera la SC el buffer
        semsignal(max);	// Si el productor está bloqueado porque el buffer estaba lleno, lo desbloquea
        sleep(rand()%VELCONS);

    }
    pthread_exit(NULL);
}
                                                             