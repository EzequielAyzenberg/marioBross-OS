/*
 * general.h
 *
 *  Created on: 25/10/2013
 *      Author: Cristian
 */

#ifndef GENERAL_H_
#define GENERAL_H_

#include<stdio.h>
#include<pthread.h>

pthread_t hiloGRID(void*(*funcion)(void*),void*parametro);

void flush_in(void);
//TECNICAMENTE borra el bufer de lectura del teclado.


#endif /* GENERAL_H_ */
