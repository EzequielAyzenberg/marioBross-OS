/*
 * Personaje.h
 *
 *  Created on: 14/12/2013
 *      Author: utnso
 */

#ifndef PERSONAJE_H_
#define PERSONAJE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <theGRID/general.h>
#include <theGRID/sockets.h>
#include <theGRID/cadenas.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <ncurses.h>



#define LOCAL_LOG "/home/utnso/GRIDLogs/LOGPersonaje/"
#define muestreo false

//Variables
typedef struct{
	t_log* debug;
	t_log* trace;
}logs;

typedef struct hilos{
	int nombre;
	char *nivel;
	pthread_t hilo;
}thilo;

typedef struct info{
	char *nivel;
}tinfo;

typedef struct personaje {
	char nombre[32];
	char simbolo;
	t_list *planDeNiveles;
	int vidas;
	int orquestadorPort;
	char *orquestadorIP;
	t_list *miniPersonajes;
	t_list *miniHilos;
}tpersonaje;

typedef struct miniPersonaje{
	char *nivel;
	t_list *planDeRecursos;
	int orquestadorSocket;
	int posX;
	int posY;
	logs *loggeo;
}tminipersonaje;

typedef struct recurso{
	char tipo;
	int posX;
	int posY;
	bool checked;
}trecurso;


//pthread_mutex_t mutexMuerte=PTHREAD_MUTEX_INITIALIZER;

int repetir,ganado,limboOK,repeticiones,interrupcion,sockfd;

tpersonaje personaje;
char *recurso, *nivelAux;
t_list *lista, *listaRecursos;
pid_t pid;
pthread_t hilo;
t_config * cfgPersonaje;


#endif /* PERSONAJE_H_ */
