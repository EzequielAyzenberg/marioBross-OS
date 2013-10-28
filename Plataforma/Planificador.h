/*
 * Planificador.h
 *
 *  Created on: 19/10/2013
 *      Author: utnso
 */

#ifndef PLANIFICADOR_H_
#define PLANIFICADOR_H_

#include <theGRID/sockets.h>
#include <stdio.h>
#include "Plataforma.h"

struct data{
	int dist;
	char recsol;
};

typedef struct Pstack{
	char recurso;
	struct Pstack* sgte;
}stack;

typedef struct Pplayer{
	int pid;
	char sym;
	struct data data;
	stack* stack;
	struct Pplayer* sgte;
}player;

typedef struct Pglobal{
	info* cabecera;
	player** rfte;
	player** rfin;
	player** sfte;
	player** sfin;
	player* exe;
	stack** recur;
	int* algo;
	int* resto;
}global;

void *planificador(void *);

#endif /* PLANIFICADOR_H_ */
