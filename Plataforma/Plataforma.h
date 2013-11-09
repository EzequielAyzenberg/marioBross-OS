/*
 * plataforma.h
 *
 *  Created on: 27/10/2013
 *      Author: utnso
 */

#ifndef PLATAFORMA_H_
#define PLATAFORMA_H_

#include <commons/collections/list.h>
#include <theGRID/sockets.h>
#include <theGRID/general.h>
#include <theGRID/cadenas.h>
#include "logger.h"
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>



/* Nodos de la lista NOVEDADES */
typedef struct tnuevo{
	int pid;
	char sym;
	struct tnuevo* sgte;
}nuevo;

/* Nodos que contienen listas de novedades */
typedef struct tnodoNivel{
	int nid;
	nuevo *tandaRaiz;
	nuevo *tandaActual;
	char name[13];
	int cantJugadores;
}nodoNivel;




/* Crea una tanda vacia con el argumento
 * apuntando a la cabeza de la lista
 */void crearTanda(nuevo**);

#endif /* PLATAFORMA_H_ */
