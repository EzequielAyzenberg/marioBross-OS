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

int connectGRID(int,char*);

int listenGRID(int);

int selectGRID(int,fd_set *);

int acceptGRID(int);

void sendHandshake(short,char*,char,short);

void sendAnswer(short,short, char, char, short);

int recvHandshake(handshake *,int);

int recvAnswer(answer *,int);

void terminar(int, int);

#endif /* SOCKETS_H_ */
