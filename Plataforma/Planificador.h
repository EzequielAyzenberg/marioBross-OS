/*
 * Planificador.h
 *
 *  Created on: 19/10/2013
 *      Author: utnso
 */

#ifndef PLANIFICADOR_H_
#define PLANIFICADOR_H_

#include<stdio.h>
#include<theGRID/sockets.h>


typedef struct tnuevo{
	int pid;
	char sym;
	struct tnuevo* sgte;
}nuevo;

typedef struct tinfo{
	int nid;
	char name[17];
	int currplay;
	nuevo* new;
	struct tinfo* sgte;
}info;

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
