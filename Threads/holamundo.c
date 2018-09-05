#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> //library for the threads

//To compile
//	gcc -o holamundo holamundo.c -lpthread

#define NTHREADS 4

void * tfunc(void * args);

int terminado = 0;


int main(){
	pthread_t tid[NTHREADS]; //store the id of the thread
	int i = 0;
	
	//id of the thread, parameters of the thread, function, params of the func
	//				with parameters NULL = default
	for(i = 0; i < NTHREADS; i++){
		pthread_create(&tid[i], NULL, tfunc, NULL); 
	}
	
	//if the principal or parent thread finish it would stop all the created threads
	for(i = 0; i < NTHREADS; i++){
		pthread_join(tid[i], NULL); 
	}
}

void * tfunc(void *args){
	sleep(5);
	printf("Hola mundo\n");
	sleep(5);
	terminado = 1;
}