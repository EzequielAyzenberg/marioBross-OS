/*
 * conexiones.c
 *
 *  Created on: 24/10/2013
 *      Author: utnso
 */
#include <stdio.h>
#include <stdlib.h>
#include <theGRID/sockets.h>
#include "conexiones.h"
#include <commons/string.h>
#include "cargador.h"
#include <sys/inotify.h>
#include "cargador.h"
#include <signal.h>
#include "personajes.h"
#include <string.h>
answer bufferAnswer;
//int a=0;
pthread_mutex_t mutexDibujar;
/*void controlador(int signal){
	int a=1;


}*/

void escucharPlanificador(datosConexiones *info){
	int a=0;
	nivelConfig bufferConfig;
	bufferConfig.path=(char*)malloc(strlen(info->config->path)+1);
	//strcpy(info.config->path,"hola");
	//printf("%s\n",info.config->path);
	strcpy(bufferConfig.path,info->config->path);
	int i=0;
	fd_set readfds,active; //necesito un mejor nombre
	int fd;
	int watch;
	int socketMAX;
	struct timeval time;
	time.tv_sec = 1;
	time.tv_usec = 0;
	int socketBuffer=info->socket;
	fd=inotify_init();//crea el inotify al comienzo de cada loop
	watch=inotify_add_watch(fd,info->config->path,IN_CLOSE_WRITE/*IN_MODIFY*/);
	//puts(info->config->path);
	FD_ZERO (&active);
	FD_SET (fd, &active);
	FD_SET (info->socket, &active);

	while(1){
	if (a==1){
		t_personaje *bufferPersonaje;
		nivel_gui_terminar();
			int i=0;
			char msg[200];
							char *itoas=malloc(1);
							char *itoas2=malloc(1);
							char *itoas3=malloc(1);
			for (i=0;i<list_size(info->listaJugadoresActivos);i++){
				bufferPersonaje=list_get(info->listaJugadoresActivos,i);
				strcpy(msg,"SEÑAL: el pj num:");

				strcat(msg,&bufferPersonaje->id);
				strcat(msg,"quiere un:");

				strcat(msg,&bufferPersonaje->recursoEspera);

							loguearInfo(msg);
			}
			exit(0);
	}
	//signal(SIGINT,&controlador);
	FD_ZERO (&active);
	FD_SET (fd, &active);
	FD_SET (info->socket, &active);

	readfds=active;
	time.tv_sec = 1;
	time.tv_usec = 0;
	//fd=inotify_init();//crea el inotify al comienzo de cada loop
	//watch=inotify_add_watch(fd,"/home/utnso/GITHUB/tp-2013-2c-the-grid/Nivel/nivel1.cfg",IN_MODIFY);

	if(info->socket>fd)socketMAX=info->socket;
	else socketMAX=fd;

	//FD_ZERO (&readfds);
	//FD_SET (fd, &readfds);
	//FD_SET (info.socket, &readfds);
	//puts("selecteando");
	select(socketMAX+1,&readfds,NULL,NULL,&time);
	i=0;
	while ((!FD_ISSET(i,&readfds))&&(i<=socketMAX+1)) i++;

	if (i==info->socket){

	recvAnswer(&bufferAnswer,info->socket);
	int ctrl=bufferAnswer.msg;
	switch(ctrl){
	case 7:if(crearPersonaje(info->listaJugadoresActivos,0,0,bufferAnswer.symbol)==1)sendAnswer(1,0,' ',' ',info->socket);
		else (sendAnswer(-1,0,' ',' ',info->socket));
	break;
	case 3:if(moverPersonaje(info->listaJugadoresActivos,bufferAnswer.cont/100,bufferAnswer.cont%100,bufferAnswer.symbol)==1)sendAnswer(1,0,' ',' ',info->socket);
		else(sendAnswer(-1,0,' ',' ',info->socket));
	break;
	case 2:if(bufferAnswer.cont){
		     if(otorgarRecurso(info->listaRecursos,info->listaJugadoresActivos,bufferAnswer.data,bufferAnswer.symbol)==1)sendAnswer(1,0,' ',' ',info->socket);
			else (sendAnswer(-1,0,' ',' ',info->socket));

			}else sendAnswer(2,chequearRecurso(info->listaRecursos,bufferAnswer.data),' ',' ',info->socket);
	break;
	case 5:sendAnswer(1,0,' ',' ',info->socket);
		   while(recvAnswer(&bufferAnswer,info->socket)==2){
		           recibirRecursos(info->listaRecursos,bufferAnswer.data);
		   }
	break;
	case 8:matarPersonaje(info->listaJugadoresActivos,info->listaJugadoresMuertos,info->listaRecursos,bufferAnswer.symbol);
	break;
		}

	}

	if(i==fd){
		//a=1;
		//puts("TODAVIA SIRVE");
		//sleep(1);
		cargarConfig(&bufferConfig);
		if(*info->config->algoritmo!=*bufferConfig.algoritmo||info->config->quantum!=bufferConfig.quantum){
			*info->config=bufferConfig;
			if(!strcmp(info->config->algoritmo,"RR")){
				sendAnswer(6,info->config->quantum,'5',' ',socketBuffer);
				}else sendAnswer(6,0,'9',' ',socketBuffer);//el argumento 3, el Remind distance esta harcodeado, CODIFICAR BIEN

		}
		if(info->config->retardo!=bufferConfig.retardo){
			*info->config=bufferConfig;
			sendAnswer(4,info->config->retardo,' ',' ',socketBuffer);//PREGUNTAR A CRIS SI ESTA BIEN
			//mandar mensaje de retardo, aun no especificado
		}

		//puts("modificaron el archivo");
	}



	inotify_rm_watch(fd,watch);
	close(fd);
	fd=inotify_init();//crea el inotify al comienzo de cada loop
	watch=inotify_add_watch(fd,info->config->path,IN_CLOSE_WRITE);
	time.tv_sec = 1;
	time.tv_usec = 0;
	pthread_mutex_lock(&mutexDibujar);

	actualizarNivel(*info->listaRecursos,*info->listaEnemigos,*info->listaJugadoresActivos,info->config->nombre);

	pthread_mutex_unlock( &mutexDibujar);

	};
}

void handshakePlataforma(datosConexiones *info){
	//char* puertoAux,ip,buffer;

	//printf ("puerto %s",info.config->orquestador);
	char* buffer;
	buffer=(char*)malloc(strlen(info->config->orquestador)+1);
	char* puertoAux;
	puertoAux=(char*)malloc(strlen(info->config->orquestador)+1);
	char* ip;
	ip=(char*)malloc(strlen(info->config->orquestador)+1);
	int puerto;
	strcpy(buffer,info->config->orquestador);
	strcpy(ip,*string_split(buffer,":"));
	strcpy(puertoAux,string_substring_from(buffer,strlen(ip)+1));
	//puts(ip);
	puerto=atoi(puertoAux);
	//printf("%d\n",puerto);
	int socketPlataforma = connectGRID(puerto,ip);
	short socketBuffer=(short)socketPlataforma; //te odio Cristian, que necesidad habia? no podias simplemente recibir un int?? ¬¬

	sendHandshake(0,info->config->nombre,' ',socketBuffer);
	recvAnswer(&bufferAnswer,socketBuffer);
//	printf("La plataforma quiere un %d\n",bufferAnswer.msg);
	if(bufferAnswer.msg==-1){
		puts("Nivel repetido");
		sleep(1);
		puts("Abortando proceso");
		sleep(1);
		puts("Proceso abortado, siempre te recordare");
		exit(1);
		}
	if(bufferAnswer.msg==6){
		//puts("La plataforma ha solitica el tipo de algoritomo, informando...");
		if(!strcmp(info->config->algoritmo,"RR")){
			sendAnswer(6,info->config->quantum,'5',' ',socketBuffer);
			}else sendAnswer(6,0,'9',' ',socketBuffer);//el argumento 3, el Remind distance esta harcodeado, CODIFICAR BIEN

		}
	answer temp;
	//HARCODEADO!!!
	recvAnswer(&temp,socketBuffer);
	sendAnswer(4,info->config->retardo,' ',' ',socketBuffer);

	info->socket=socketBuffer;
	escucharPlanificador(info);
}


