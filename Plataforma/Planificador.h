/*
 * Planificador.h
 *
 *  Created on: 19/10/2013
 *      Author: utnso
 */

#ifndef PLANIFICADOR_H_
#define PLANIFICADOR_H_


#include "Plataforma.h"

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

struct algo{
	int algo;
	int remainDist;
	int retardo;
};

typedef struct execution{
	t_player* player;
	int rem_cuantum;
}t_exec;

typedef struct t_logs{
	t_log*trace;
	t_log*debug;
	t_log*warning;
}logs;

typedef struct Pglobal{
	nodoNivel* cabecera;
	t_list*ready;
	t_list*sleeps;
	t_exec*exe;
	t_list*recur;
	fd_set*original;
	logs logging;
	int*maxfd;
	struct algo*algo;
	bool playing;
}global;


void *planificador(void *);

#endif /* PLANIFICADOR_H_ */
