#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

#define NUMTHREADS 2
#define TRUE 1
#define FALSE 0

void * tfunc(void * args);
void tomando_cerveza(int n);
void usando_el_wc(int n);


int flag[2] = {FALSE, FALSE};
int turno = 0;

int main(){
	pthread_t tid[NUMTHREADS];
	int args[NUMTHREADS];
	int i;
	
	srand(getpid());
	
	for(i = 0; i < NUMTHREADS; i++){
		args[i] = i;
		pthread_create(&tid[i], NULL, tfunc, &args[i]);
	}
	
	for(i = 0; i < NUMTHREADS; i++){
		pthread_join(tid[i],NULL);
	}
}

void * tfunc(void * args){
	int i = *((int *) args);
	int j = 1 - i;
	int n;
	
	printf("Soy el hilo %d, i=%d, j=%d\n", i,i,j);
	
	for(n = 0; n < 10; n++){
		tomando_cerveza(i);
		//Seccion de entrada
		flag[i] = TRUE;
		//sleep(1);
		while(flag[j]){	//Si el proceso j tiene la bandera encendida, me espero
			flag[i] = FALSE;
			sleep(1);
			flag[j] = TRUE;
		}
		
		
		usando_el_wc(i);
		
		//Seccion de salida
		flag[i] = FALSE;		//ceder el turno al turno
	}
}

void tomando_cerveza(int n){
	printf("Tomando cerveza %d\n", n);
	usleep(rand()%3000000);
}
void usando_el_wc(int n){
	printf("Entra al wc %d\n", n);
	usleep(rand()%3000000);
	printf("Sale del wc %d\n", n);
}