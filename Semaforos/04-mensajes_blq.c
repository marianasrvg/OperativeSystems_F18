#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/msg.h>
#include <wait.h>

// Definir una estructura que se va a usar para mandar los mensajes
typedef struct {
	long msg_type;		// Tipo de mensaje, debe ser long
	char mensaje[100];	// Contenido
	} MSGTYPE; 

int msgqid;

void emisor();
void receptor();

int main()
{
	int pid;
	int i;
	
	// Crear un buzón o cola de mensajes
	msgqid=msgget(0x1234,0666|IPC_CREAT);
	
	// Crear un nuevo proceso
	pid=fork();
	if(pid==0)
		emisor(); // Se ejecuta el emisor en el proceso hijo

	pid=fork();
	if(pid==0)
		receptor();	// El padre es el receptor

	// Uno para el emisor, otro para el receptor
	for(i=0;i<2;i++)
		wait(NULL); 

	printf("Procesos terminados\n");
	msgctl(msgqid,IPC_RMID,NULL);
	return(0);
}

void emisor()
{
	int i;
	MSGTYPE m;
	for(i=0;i<10;i++)
	{
		m.msg_type=1;
		sprintf(m.mensaje,"Hola, este es el mensaje %d",i);
		msgsnd(msgqid,&m,sizeof(MSGTYPE)-sizeof(long),0);
		printf("\t\t\t\tMensaje enviado\n");
		sleep(1);
	}
	m.msg_type=1;
	strcpy(m.mensaje,"Fin");
	msgsnd(msgqid,&m,sizeof(MSGTYPE)-sizeof(long),0);
		
	exit(0);	// Termina el proceso
}

void receptor()
{
	MSGTYPE m;	// Donde voy a recibir el mensaje
	int retval;
	
	do
	{
		retval=msgrcv(msgqid,&m,sizeof(MSGTYPE)-sizeof(long),1,0);
		printf("Recibido: %s\n",m.mensaje);
		sleep(3);
	}
	while(strcmp(m.mensaje,"Fin")!=0); // Hasta que el mensaje sea "Fin"

	exit(0);	// Termina el proceso

}
