/*
 * encoding.h
 *
 *  Created on: 13/10/2013
 *      Author: alumno
 */

#ifndef ENCODING_H_
#define ENCODING_H_

typedef struct {
	char type;
	char name[8];
	char symbol;
} t_handshake;

typedef struct {
	char msj;
	short cont;
	char data;
	char symbol;
} t_answer;

typedef struct {
	int lenght;
	char *data;
} t_stream;

t_stream *handshakeSerializer(t_handshake *self);
t_handshake *handshakeDeserializer(t_stream *stream);
t_handshake handshakeNew(char tipo, char *name, char simbolo);
t_stream *answerSerializer(t_answer *self);
t_answer *answerDeserializer(t_stream *stream);
t_answer answerNew(char mensaje, short cont, char data, char simbolo);

#endif /* ENCODING_H_ */
