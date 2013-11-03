/*
 * orquestador.h
 *
 *  Created on: 27/10/2013
 *      Author: utnso
 */

#ifndef ORQUESTADOR_H_
#define ORQUESTADOR_H_

#include "Plataforma.h"
#include <stdbool.h>

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
void responderError(int socketJugador);
nodoNivel *validarNivel(char nombreNivel[13],t_list* listaNiveles);
nodoNivel *buscarNivelEnSistema(char nombreNivel[13],t_list* listaNiveles);
void crearTanda(nuevo** lista);
void reconectarNivel(nodoNivel *nodo,int nid);
void agregarNivel(handshake handshakeNivel,int socketNivel, t_list* listaNiveles);
bool _hay_jugadores(nodoNivel *nivel);
bool chequearKoopa(t_list *ganadores, t_list* listaNiveles);
void activarKoopa(void);

#endif /* ORQUESTADOR_H_ */
