/*
 * personajes.c
 *
 *  Created on: 01/11/2013
 *      Author: utnso
 */
#include "personajes.h"
#include "cargador.h"
#include <stdio.h>
#include <stdlib.h>
#include <commons/collections/list.h>

int crearPersonaje(t_list* listaJugadoresActivos,int x,int y,char id){//IMPLEMENTAR QUE REVISE SI ESE PERSONAJE NO EXISTIA YA

	t_personaje *bufferPj;



	bool _is_Personaje(t_personaje* pj2){
								if(pj2->id==id)return true;
								return false;
							}

	if(list_find(listaJugadoresActivos,(void*)_is_Personaje)==NULL){


		bufferPj=malloc(16);
		bufferPj->id=id;
		bufferPj->posx=x;
		bufferPj->posy=y;
	bufferPj->recursos=list_create();
		list_add(listaJugadoresActivos,bufferPj);
		return 1;
	}else return -1;









/*

	int i=0;
	personaje *buffer;
	personaje *buffer2;
	personaje *buffer3;
	buffer2=(personaje*)malloc(sizeof(personaje));
	/*buffer2=malloc(sizeof(personaje));
	buffer3=buffer2;
	buffer2->id=' ';
	(*buffer).id=id;
	(*buffer).posx=x;
	(*buffer).posy=y;
	(*buffer).recursos=NULL;*/
	/*
	while(i<list_size(listaJugadoresActivos)){

			buffer2=list_get(listaJugadoresActivos,i);
			if(buffer->id==buffer2->id)break;
			i++;
	}
	if(buffer->id!=buffer2->id){
		free(buffer3);
		list_add(listaJugadoresActivos,buffer);
		return 1;
	}
	else {
		free(buffer3);
		free(buffer);
		return-1;
	}
*/}

int moverPersonaje(t_list* listaJugadoresActivos,int x,int y,char id){
	int i=0;
	t_personaje* buffer;

	buffer=malloc(sizeof(personaje));
	buffer->id=' ';
	while(i<list_size(listaJugadoresActivos)){

		buffer=list_get(listaJugadoresActivos,i);
		if(buffer->id==id)break;
		i++;
	}
	if(buffer->id!=id)return -1;
	else {
			buffer->posx=x;
			buffer->posy=y;
			return 1;

	}
}

int otorgarRecurso(t_list* listaCajas,t_list* listaPersonajes,char recurso,char pj){
	/*
	Caja *buffer;
	personaje *bufferPj;
	bool _is_Recurso(Caja *caja){
						if(caja->id==recurso)return true;
						return false;
	}
	buffer=list_find(listaCajas,(void*)_is_Recurso);
	if(buffer->quantity>0){

		bool _is_Personaje(personaje* pj2){
							if(pj2->id==pj)return true;
							return false;
						}

		bufferPj=list_find(listaPersonajes,(void*)_is_Personaje);
		if(bufferPj!=NULL){
			char* bufferRecurso=malloc(1);
			*bufferRecurso=recurso;
			buffer->quantity--;
			list_add(bufferPj->recursos,bufferRecurso);
			return 1;
		}

		else return -1;


	}else  return -1;
*/

}
