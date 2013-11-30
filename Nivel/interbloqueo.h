/*
 * interbloqueo.h
 *
 *  Created on: 23/11/2013
 *      Author: utnso
 */

#ifndef INTERBLOQUEO_H_
#define INTERBLOQUEO_H_
#include <commons/collections/list.h>
typedef struct estructuraInterbloqueo{
	t_list *listaRecursos;
	t_list *listaJugadoresMuertos;
	t_list *listaJugadores;
	int tiempoEspera;
	short *socket;
}infoInterbloqueo;

void controlInterbloqueo(infoInterbloqueo *info);

#endif /* INTERBLOQUEO_H_ */
