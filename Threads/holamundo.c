#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> //library for the threads

//To compile
//	gcc -o holamundo holamundo.c -lpthread

void * tfunc(void * args);

int terminado = 0;


int main(){
	pthread_t tid; //store the id of the thread
	
	//id of the thread, parameters of the thread, function, params of the func
	//				with parameters NULL = default
	pthread_create(&tid, NULL, tfunc, NULL); 
	//if the principal or parent thread finish it would stop all the created threads
	pthread_join(tid,NULL);
	
}

void * tfunc(void *args){
	sleep(5);
	printf("Hola mundo\n");
	sleep(5);
	terminado = 1;
}