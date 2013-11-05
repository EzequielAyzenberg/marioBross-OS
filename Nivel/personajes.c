/*
 * personajes.c
 *
 *  Created on: 01/11/2013
 *      Author: utnso
 */
#include "personajes.h"
#include <stdio.h>
#include <stdlib.h>
#include <commons/collections/list.h>

void crearPersonaje(t_list* listaJugadoresActivos,int x,int y,char id){
	personaje* buffer;
	buffer=malloc(sizeof(personaje));
	(*buffer).id=id;
	(*buffer).posx=x;
	(*buffer).posy=y;
	list_add(listaJugadoresActivos,buffer);
}
