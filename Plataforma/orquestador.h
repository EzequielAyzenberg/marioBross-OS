/*
 * orquestador.h
 *
 *  Created on: 27/10/2013
 *      Author: utnso
 */

#ifndef ORQUESTADOR_H_
#define ORQUESTADOR_H_

static void *orquestador(infoOrquestador info);
void clienteNuevo(handshake nuevoHandshake,int socketJugador);
void clienteViejo(handshake nuevoHandshake,int socketJugador);


#endif /* ORQUESTADOR_H_ */
