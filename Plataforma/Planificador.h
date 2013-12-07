/*
 * Planificador.h
 *
 *  Created on: 19/10/2013
 *      Author: utnso
 */

#ifndef PLANIFICADOR_H_
#define PLANIFICADOR_H_

#include "Plataforma.h"

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
	t_exec*exe;
	t_list*recur;
	set*original;
	logs logging;
	int*maxfd;
	struct algo*algo;
	bool playing;
}global;


void *planificador(void *);

#endif /* PLANIFICADOR_H_ */
