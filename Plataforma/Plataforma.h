/*
 * plataforma.h
 *
 *  Created on: 27/10/2013
 *      Author: utnso
 */

#ifndef PLATAFORMA_H_
#define PLATAFORMA_H_
#define LOCAL_LOG "/home/utnso/GRIDLogs/LOGPlataforma/"

#include <commons/collections/list.h>
#include <commons/config.h>
#include <commons/log.h>
#include <theGRID/sockets.h>
#include <theGRID/general.h>
#include <theGRID/cadenas.h>
#include <theGRID/logger.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include "GUI/Introduccion.h"

#define LOCAL_LOG "/home/utnso/GRIDLogs/LOGPlataforma/"
bool finalizar;

/* Nodos de la lista NOVEDADES */
typedef struct tnuevo{
	int pid;
	char sym;
	struct tnuevo* sgte;
}nuevo;
/* Nodos que contienen listas de novedades */

struct data{
	int pos;
	int dist;
	char recsol;
};

typedef struct Pstack{
	char recurso;
}t_stack;

typedef struct Pplayer{
	int pid;
	char sym;
	struct data data;
	t_list* t_stack;
}t_player;

typedef struct execution{
	t_player* player;
	int rem_cuantum;
}t_exec;

struct algo{
	int algo;
	int remainDist;
	int retardo;
};

typedef struct tnodoNivel{
	int nid;
	nuevo *tandaRaiz;
	nuevo *tandaActual;
	char name[13];
	int cantJugadores;
	pthread_t idHilo;
	t_list*ready;
	t_list*sleeps;
	t_list*deads;
	t_exec*exe;
	struct algo*algo;
}nodoNivel;

/* Crea una tanda vacia con el argumento
 * apuntando a la cabeza de la lista
 */void crearTanda(nuevo**);
   int cargarRemainingDistance(char * CFG_PATH);

#endif /* PLATAFORMA_H_ */
