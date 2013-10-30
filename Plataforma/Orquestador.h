/*
 * orquestador.h
 *
 *  Created on: 27/10/2013
 *      Author: utnso
 */

#ifndef ORQUESTADOR_H_
#define ORQUESTADOR_H_

#include "Plataforma.h"

/* Nodos de la lista Ganadores */
typedef struct{
	char personaje;
}jugadorGanador;

/* Registro que necesito el orquestador
 * al ser llamado por la plataforma
 */typedef struct tinfoOrquestador{
	 int puerto;
	 t_list *listaNiveles;
 }infoOrquestador;

void *orquestador(void* info);
void  nivelNuevo(handshake handshakeNivel,  int socketNivel,   t_list* listaNiveles);
void clienteNuevo(handshake handshakeJugador,int socketJugador, t_list* listaNiveles);
void clienteViejo(handshake handshakeJugador, t_list *ganadores);
void crearHiloPlanificador(handshake handshakeNivel,nodoNivel* nivel);
void responder(int socketJugador);
nuevo** validarNivel(char nombreNivel[13],t_list* listaNiveles);
void crearTanda(nuevo** lista);

#endif /* ORQUESTADOR_H_ */
