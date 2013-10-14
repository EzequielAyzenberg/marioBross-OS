/*
 * encoding.c
 *
 *  Created on: 13/10/2013
 *      Author: alumno
 */

#include "encoding.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

t_stream *handshakeSerializer(t_handshake *self){
	char *data = (char *)malloc( sizeof(self->type) + strlen(self->name) + 1 + sizeof(self->symbol) );
	t_stream *stream = malloc(sizeof(t_stream));
	int offset = 0, tmp_size = 0;

	memcpy(data + offset, &self->type, tmp_size = sizeof(self->type) );

	offset += tmp_size;
	memcpy(data + offset, self->name, tmp_size = sizeof(self->name) );

	offset += tmp_size;
	memcpy(data + offset, &self->symbol, tmp_size = sizeof(self->symbol) );

	stream->lenght = offset + tmp_size;
	stream->data = data;

	return stream;
}

t_handshake *handshakeDeserializer(t_stream *stream){
	t_handshake *self = malloc( sizeof(t_handshake) );
	int offset = 0, tmp_size = 0;

	memcpy(&self->type, stream->data + offset, tmp_size = sizeof(self->type) );

	offset += tmp_size;
	*self->name = (char(*)[8])malloc(tmp_size);
	memcpy(self->name, stream->data + offset, tmp_size = sizeof(self->name));

	offset += tmp_size;
	memcpy(&self->symbol, stream->data + offset, tmp_size = sizeof(self->symbol) );

	return self;
}

t_handshake handshakeNew(char tipo, char *name, char simbolo){
	t_handshake nuevo;
	nuevo.type = tipo;
	strncpy(nuevo.name,name,8);
	nuevo.name[7] = '\0';
	nuevo.symbol = simbolo;
	return nuevo;
}

t_stream *answerSerializer(t_answer *self){
	char *data = (char *)malloc( sizeof(self->msj) + sizeof(self->cont) + sizeof(self->data) + sizeof(self->symbol) );
	t_stream *stream = malloc(sizeof(t_stream));
	int offset = 0, tmp_size = 0;

	memcpy(data, &self->msj, tmp_size = sizeof(self->msj) );

	offset += tmp_size;
	memcpy(data + offset, &self->cont, tmp_size = sizeof(self->cont) );

	offset += tmp_size;
	memcpy(data + offset, &self->data, tmp_size = sizeof(self->data) );

	offset += tmp_size;
	memcpy(data + offset, &self->symbol, tmp_size = sizeof(self->symbol) );

	stream->lenght = offset + tmp_size;
	stream->data = data;

	return stream;
}

t_answer *answerDeserializer(t_stream *stream){
	t_answer *self = malloc( sizeof(t_answer) );
	int offset = 0, tmp_size = 0;

	memcpy(&self->msj, stream->data, tmp_size = sizeof(self->msj) );

	offset += tmp_size;
	memcpy(&self->cont, stream->data + offset, tmp_size = sizeof(self->cont) );

	offset += tmp_size;
	memcpy(&self->data, stream->data + offset, tmp_size = sizeof(self->data) );

	offset += tmp_size;
	memcpy(&self->symbol, stream->data + offset, tmp_size = sizeof(self->symbol) );

	return self;
}

t_answer answerNew(char mensaje, short cont, char data, char simbolo){
	t_answer nuevo;
	nuevo.msj = mensaje;
	nuevo.cont = cont;
	nuevo.data = data;
	nuevo.symbol = simbolo;
	return nuevo;
}


