/*
 * headers.h
 *
 *  Created on: 09/12/2013
 *      Author: utnso
 */

#ifndef HEADERS_H_
#define HEADERS_H_
#define LOCAL_LOG "/home/utnso/GRIDLogs/LOGPlataforma/"

//#include"Plataforma.h"
#include"GUI/Introduccion.h"
#include"Pantalla.h"
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
#include <wait.h>

//General---------------------------------------------

t_list *listaNiveles;

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
	t_list*inters;
	t_exec*exe;
	struct algo*algo;
	bool dibujar;
}nodoNivel;
//----------------------------------------------------


//planificador----------------------------------------
typedef struct t_logs{
	t_log*trace;
	t_log*debug;
	t_log*info;
}logs;

typedef struct t_set{
	fd_set*original;
}set;

typedef struct Pglobal{
	nodoNivel* cabecera;
	t_list*ready;
	t_list*sleeps;
	t_list*deads;
	t_list*inters;
	t_exec*exe;
	t_list*recur;
	set*original;
	logs logging;
	int*maxfd;
	struct algo*algo;
	bool playing;
}global;
//----------------------------------------------------


//Orquestador-----------------------------------------
/* Nodos de la lista Ganadores */
typedef struct{
	char personaje;
}jugadorGanador;

/* Nodos de la lista de planificadores */
typedef struct{
	pthread_t idHilo;
}nodoPlanificador;

 // Registro del archivo cfg del orquestador
  typedef struct{
	 char * ip;
	 char * koopa;
	 char * script;
	 int puerto;
 }cfgOrquestador;
 //----------------------------------------------------

#endif /* HEADERS_H_ */
