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

answer bufferAnswer;

void escucharPlanificador(datosConexiones info){
	int i=0;
	while(1){

	recvAnswer(&bufferAnswer,info.socket);
	int ctrl=bufferAnswer.msg;
	switch(ctrl){
	case 7:if(crearPersonaje(info.listaJugadoresActivos,0,0,bufferAnswer.symbol)==1)sendAnswer(1,0,' ',' ',info.socket);
		else (sendAnswer(-1,0,' ',' ',info.socket));
	break;
	case 3:if(moverPersonaje(info.listaJugadoresActivos,bufferAnswer.cont/100,bufferAnswer.cont%100,bufferAnswer.symbol)==1)sendAnswer(1,0,' ',' ',info.socket);
		else(sendAnswer(-1,0,' ',' ',info.socket));
	break;
	/*case 2:if(bufferAnswer.cont){
			if(otorgarRecurso(info.listaRecursos,info.listaJugadoresActivos,bufferAnswer.data,bufferAnswer.symbol)==1)sendAnswer(1,0,' ',' ',info.socket);
			else (sendAnswer(-1,0,' ',' ',info.socket));
	break;
			}*/
		}

	}
};

void handshakePlataforma(datosConexiones info){
	//char* puertoAux,ip,buffer;

	char* buffer;
	buffer=(char*)malloc(sizeof(info.config.orquestador));
	char* puertoAux;
	puertoAux=(char*)malloc(sizeof(info.config.orquestador));
	char* ip;
	ip=(char*)malloc(sizeof(info.config.orquestador));
	int puerto;
	strcpy(buffer,info.config.orquestador);
	strcpy(ip,*string_split(buffer,":"));
	strcpy(puertoAux,string_substring_from(buffer,strlen(ip)+1));
	//puts(ip);
	puerto=atoi(puertoAux);
	//printf("%d\n",puerto);
	int socketPlataforma = connectGRID(puerto,ip);
	short socketBuffer=(short)socketPlataforma; //te odio Cristian, que necesidad habia? no podias simplemente recibir un int?? ¬¬
	sendHandshake(0,info.config.nombre,' ',socketBuffer);
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
		if(!strcmp(info.config.algoritmo,"RR")){
			sendAnswer(6,info.config.quantum,'5',' ',socketBuffer);
			}else sendAnswer(6,0,'9',' ',socketBuffer);//el argumento 3, el Remind distance esta harcodeado, CODIFICAR BIEN

		}
	info.socket=socketBuffer;
	escucharPlanificador(info);
}


