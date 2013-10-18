/*
 * sockets.h
 *
 *  Created on: 09/10/2013
 *      Author: utnso
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

struct typeHand{
	char type;
	char name[9];
	char symbol;
}__attribute__((packed));

struct typeAns{
	char msg;
	short cont;
	char data;
	char symbol;
}__attribute__((packed));

typedef struct typeHand handshake;
typedef struct typeAns answer;

int connectGRID(int,char*);

int listenGRID(int);

int selectGRID(int,fd_set *);

int acceptGRID(int);

void sendHandshake(char,char*,char,short);

void sendAnswer(char,short, char, char, short);

char recvHandshake(handshake *,short);

char recvAnswer(answer *,short);

void terminar(int, int);

void flush_in(void);

#endif /* SOCKETS_H_ */
