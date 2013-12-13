/*
 * personajes.h
 *
 *  Created on: 01/11/2013
 *      Author: utnso
 */

#ifndef PERSONAJES_H_
#define PERSONAJES_H_
#include  <commons/collections/list.h>

typedef struct infoPersonajes{
	int posx;
	int posy;
	char id;

}personaje;

typedef struct infoPersonajes2{
	int posx;
	int posy;
	char id;
	t_list *recursos;
	char recursoEspera;
}t_personaje;


#endif /* PERSONAJES_H_ */
