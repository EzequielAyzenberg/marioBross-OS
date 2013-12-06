/*
 * conexiones.h
 *
 *  Created on: 24/10/2013
 *      Author: utnso
 */

#ifndef CONEXIONES_H_
#define CONEXIONES_H_
#include <commons/collections/list.h>
#include "cargador.h"

typedef struct{
	nivelConfig* config;
	t_list* listaJugadoresActivos;
	t_list* listaJugadoresMuertos;
	t_list* listaRecursos;
	t_list* listaEnemigos;
	short socket;
}datosConexiones;

#endif /* CONEXIONES_H_ */
