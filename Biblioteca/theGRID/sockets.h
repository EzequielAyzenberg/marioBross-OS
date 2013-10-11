/*
 * sockets.h
 *
 *  Created on: 09/10/2013
 *      Author: utnso
 */

#ifndef SOCKETS_H_
#define SOCKETS_H_

typedef struct {
	char type;
	char name[8];
	char symbol;
}handshake;

typedef struct {
	char msg;
	short cont;
	char data;
	char symbol;
}answer;

short connectGRID(short,char*);

void sendHandshake(char,char*,char,short);

void sendAnswer(char,short, char, char, short);

char recvHandshake(handshake *,short);

char recvAnswer(answer *,short);

void terminar(short, short);

void flush_in(void);

#endif /* SOCKETS_H_ */
