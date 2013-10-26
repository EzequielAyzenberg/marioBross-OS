//SOY EL PADRE DE TODO LO QUE PASA AQUI!!!

#include<stdio.h>
#include"Planificador.h"
#include<pthread.h>


//Por ahora será un servidor SUPER HARCODEADO para testear mi planificador...


#define MYPORT 2506
#define MYIP "192.168.0.11"
#define MAX 64

void crearTanda(nuevo**);

int main(int argc, char *argv[]){
					//Carga la IP de destino y el puerto puestos a la hora de ejecutar el programa.
	int puerto;
	switch (argc){
	case 2:
		puerto=atoi(argv[1]);
		printf("Se usara el siguiente puerto:\t%d\n", puerto);
		break;
	default:
		puerto=MYPORT;
		puts("Se usara el puerto estandar.");
		break;
	}
					//Aca arranca la "Plataforma"..
	info registro;
	int listener=listenGRID(puerto);
	int nivel=acceptGRID(listener);
	puts("Tenemos un nivel conectado!!");
	registro.nid=nivel;
	strcpy(registro.name,"El Peor Nivel");
	registro.currplay=0;
	registro.new=NULL;
	registro.sgte=NULL;

	/*pthread_t idHilo;
	if(pthread_create(&idHilo, NULL, planificador, (void*)&registro)==0)puts("Hilo creado correctamente.");
	else puts("Hubo un problema en la creacion del hilo.");*/
	puts("Atendiendo Personajes ahora..");
	crearTanda(&(registro.new));
	int personaje;
	handshake temp;
	nuevo *actual;
	actual=registro.new;
	while(1){
		personaje=acceptGRID(listener);
		puts("Se ha recibido un nuevo Personaje");
		puts("Recibiendo info del Personaje..");
		recvHandshake(&temp,personaje);
		actual->pid=personaje;
		actual->sym=temp.symbol;
		if (actual->sgte==NULL)	crearTanda(&(actual->sgte));
		actual=actual->sgte;
	}





	return 0;
}

void crearTanda(nuevo** lista){
	int i;
	nuevo *tempo,*aux;
	aux=*lista;
	for (i=1;i<=5;i++){
		tempo=(nuevo*)malloc(sizeof(nuevo));
		tempo->pid=0;
		tempo->sym=' ';
		tempo->sgte=NULL;
		if(*lista==NULL)*lista=tempo;
		else{
			aux->sgte=tempo;
			aux=tempo;
		}
	}
}

