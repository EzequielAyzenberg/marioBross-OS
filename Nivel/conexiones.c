/*
 * conexiones.c
 *
 *  Created on: 24/10/2013
 *      Author: utnso
 */
#include <stdio.h>
#include <stdlib.h>
#include <sockets.h>
#include "conexiones.h"
#include <commons/string.h>
#include "cargador.h"

handshakePlataforma(nivelConfig config){
	//char* puertoAux,ip,buffer;
	char* buffer;
	buffer=(char*)malloc(sizeof(config.orquestador));
	char* puertoAux;
	puertoAux=(char*)malloc(sizeof(config.orquestador));
	char* ip;
	ip=(char*)malloc(sizeof(config.orquestador));
	int puerto;
	strcpy(buffer,config.orquestador);
	strcpy(ip,*string_split(buffer,":"));
	strcpy(puertoAux,string_substring_from(buffer,strlen(ip)+1));
	puts(ip);
	puerto=atoi(puertoAux);
	printf("%d\n",puerto);
	connectGRID(puerto,ip);
}
