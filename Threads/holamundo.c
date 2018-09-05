#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> //library for the threads

//To compile
//	gcc -o holamundo holamundo.c -lpthread

#define NTHREADS 4

//there is no other way to declare tfunc
void * tfunc(void * args);

int cont = 0;
int contparc[NTHREADS];

pthread_mutex_t l;

int main(){
	pthread_t tid[NTHREADS]; //store the id of the thread
	int args[NTHREADS];
	int i = 0;
	
	pthread_mutex_init(&l, NULL);
	
	for(i = 0; i < NTHREADS; i++){
		contparc[i] = 0;
	}
	
	//id of the thread, parameters of the thread, function, params of the func
	//				with parameters NULL = default
	for(i = 0; i < NTHREADS; i++){
		args[i] = i;
		pthread_create(&tid[i], NULL, tfunc, (void *) &args[i]);
		//usleep(1000);
	}
	
	//if the principal or parent thread finish it would stop all the created threads
	for(i = 0; i < NTHREADS; i++){
		pthread_join(tid[i], NULL); 
	}
	pthread_mutex_destroy(&l);
	
	for(i = 0; i < NTHREADS; i++){
		cont+=contparc[i];
	}
	
	printf("cont = %d\n", cont);
}

void * tfunc(void *args){
	//args is a pointer to a void
	//you have to cast it to a integer pointer
	//with the * you get the value
	
	int tnum = *((int *) args);
	
	int i, lcont = 0;

	printf("Hola mundo, soy %d\n", tnum);
	
	for(i = 0; i < 1000000000; i++){
		contparc[tnum]++
}
		
		