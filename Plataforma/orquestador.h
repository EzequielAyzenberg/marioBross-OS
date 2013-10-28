/*
 * orquestador.h
 *
 *  Created on: 27/10/2013
 *      Author: utnso
 */

#ifndef ORQUESTADOR_H_
#define ORQUESTADOR_H_

#include "Plataforma.h"
#include "Planificador.h"
#include <pthread.h>
#include <theGRID/sockets.h>

/* Nodos de la lista Ganadores */
typedef struct{
	char personaje;
}jugadorGanador;

static void *orquestador(infoOrquestador info);
void   nivelNuevo(handshake handshakeNivel,  int socketNivel,   t_list* listaNiveles);
void clienteNuevo(handshake handshakeJugador,int socketJugador, t_list* listaNiveles);
void clienteViejo(handshake handshakeJugador, t_list *ganadores);
void crearHiloPlanificador(handshake handshakeNivel,int socketNivel);


#endif /* ORQUESTADOR_H_ */
