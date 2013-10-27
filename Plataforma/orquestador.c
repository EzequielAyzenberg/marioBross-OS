/*
 * orquestador.c
 *
 *  Created on: 27/10/2013
 *      Author: EzequielAyzenberg
 */

/* Lista de Eventos tipo Analisis de Sistemas xD
 *
 * --DELEGACION DE CONEXIONES--
 *
 * RecepcionDeConexiones
 * ClienteNuevo
 * ClienteViejo
 * CrearHiloPlanificador
 * NivelNuevo
 * ValidarNiver xD
 * CrearNuevaTanda --OK--
 *
 * --KOOPA--
 *
 * ChequearKoopa
 * ActivarKoopa
 * MatarHilos
 */

#include "orquestador.h"
#include "Plataforma.h"
#include <theGRID/sockets.h>


static void *orquestador(infoOrquestador info){
	int socketOrquestador, socketIngresante;
	handshake nuevoHandshake;

	while(1){
		socketOrquestador = listenGRID(info.puerto);
		socketIngresante = acceptGRID(socketOrquestador);
		switch ( recvHandshake(&nuevoHandshake,socketIngresante) ){
		case 0: nivelNuevo(nuevoHandshake,socketIngresante); break;
		case 1: clienteNuevo(nuevoHandshake,socketIngresante); break;
		case 2: clienteViejo(nuevoHandshake,socketIngresante);
		}
	}
}

void nivelNuevo(handshake nuevoHandshake,int socketNivel){
	info registro;
		puts("Tenemos un nivel conectado!!");
		registro.nid=socketNivel;
		strcpy(registro.name,"El Peor Nivel");
		registro.currplay=0;
		registro.new=NULL;
		registro.sgte=NULL;
		pthread_t idHilo;
			if(pthread_create(&idHilo, NULL, planificador, (void*)&registro)==0)
			puts("Hilo creado correctamente.");
			else puts("Hubo un problema en la creacion del hilo.");
};

void clienteNuevo(handshake nuevoHandshake,int socketJugador){};
void clienteViejo(handshake nuevoHandshake,int socketJugador){};
