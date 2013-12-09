/*
 * sockets.h
 *
 *  Created on: 09/10/2013
 *      Author: Cristian
 */

#ifndef SOCKETS_H_
#define SOCKETS_H_

#include<arpa/inet.h>
#include<errno.h>
#include<netdb.h>
#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>
#include"general.h"

struct typeHand{
	short type;
	char name[13];
	char symbol;
}__attribute__((packed));

struct typeAns{
	short msg;
	short cont;
	char data;
	char symbol;
}__attribute__((packed));


typedef struct typeHand handshake;
typedef struct typeAns answer;

/* Le pasas el puerto y la IP(en string)
 * y el chabon te devuelve el socket ya conectado.
 */int connectGRID(int,char*);

/* Le pasas el puerto y el chabon te devuelve
 * el socket escuchando.
 */int listenGRID(int,char*);

/* Le pasas el maxfd y la lista copia de fd_set
 * y te devuelve lo que el select, sin errores.
 */int selectGRID(int,fd_set *);

/* Simple...Le pasas el Listener y te devuelve
 * un nuevo socket conectado.
 */int acceptGRID(int,int);

/* Le pasas los datos del Handshake más el
 * socket destino en formato short (castearlo).
 * --Nombre de un maximo de 12 Caracteres.
 */void sendHandshake(short,char*,char,short);

/* Lo mismo que el anterior pero con Answer.
 */void sendAnswer(short,short, char, char, short);

/* Le pasas una estructura Handshake vacia
 * (por referencia) más el socket, y te da
 * el estructura->Type como resultado o 0
 * si se desconecto.
 */int recvHandshake(handshake *,int);

/* Lo mismo que el anterior pero con
 * una estructura Answer.
 * Te devuelve el estructura->Msg como
 * resultado o 0 si se desconecto.
 */int recvAnswer(answer *,int);

/* Funciones internas para terminar el
 * programa por error.
 */void terminar(int, int);

#endif /* SOCKETS_H_ */
