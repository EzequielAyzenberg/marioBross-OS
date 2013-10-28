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
	int dist;
	char recsol;
};

typedef struct Pstack{
	char recurso;
}stack;

typedef struct Pplayer{
	int pid;
	char sym;
	struct data data;
	t_list* stack;
}player;

typedef struct Pglobal{
	nodoNivel* cabecera;
	t_list*ready;
	t_list*sleeps;
	player* exe;
	t_list*recur;
	int* algo;
	int* resto;
}global;

void *planificador(void *);

#endif /* PLANIFICADOR_H_ */
