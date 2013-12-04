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
#include "semaforos.h"
#include <theGRID/general.h>
#include <theGRID/sockets.h>

pthread_mutex_t mutexMatarPersonaje;
pthread_mutex_t mutexCrearPersonaje;

int otorgarRecurso(t_list* listaCajas,t_list* listaPersonajes,t_list* listaPersonajesBloqueados,char recurso,char pj){

	Caja *buffer;
	t_personaje *bufferPj;
	bool _is_Recurso(Caja *caja){
						if(caja->id==recurso)return true;
						return false;
	}
	buffer=list_find(listaCajas,(void*)_is_Recurso);
	if(buffer->quantity>0){

		bool _is_Personaje(t_personaje* pj2){
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


	}
	else  {

		bool _is_Personaje(t_personaje* pj2){
									if(pj2->id==pj)return true;
									return false;
								}

		bufferPj=list_find(listaPersonajes,(void*)_is_Personaje);
		if(bufferPj!=NULL){
			bufferPj->recursoEspera=recurso;
			list_add(listaPersonajesBloqueados,bufferPj);
			//printf("%c esta esperando una %c",bufferPj->id,bufferPj->recursoEspera);
		}
		return -1;
	}


}

int chequearRecurso(t_list* listaCajas,char recurso){
	Caja *buffer;
	bool _is_Recurso(Caja *caja){
							if(caja->id==recurso)return true;
							return false;
		}
	buffer=list_find(listaCajas,(void*)_is_Recurso);
	return (buffer->posx*100+buffer->posy);
}

int recibirRecursos(t_list* listaCajas,t_list* listaPersonajesBloqueados,char recurso,short socket){
	Caja *buffer;
	t_personaje *bufferPersonaje;
	bool _is_Recurso(Caja *caja){
								if(caja->id==recurso)return true;
								return false;
			}
	buffer=list_find(listaCajas,(void*)_is_Recurso);

	bool _is_Recurso_En_Espera(t_personaje *personaje){
									if(personaje->recursoEspera==recurso)return true;
									return false;
				}

	bufferPersonaje=list_find(listaPersonajesBloqueados,(void*)_is_Recurso_En_Espera);
	if (bufferPersonaje!=NULL){
		bufferPersonaje->recursoEspera=' ';
		sendAnswer(2,1,recurso,bufferPersonaje->id,socket);
		bool _is_Personaje(t_personaje* pj){
							if(pj->id==bufferPersonaje->id)return true;
							return false;
						}
		list_remove_by_condition(listaPersonajesBloqueados,(void*)_is_Personaje);

	}


	else buffer->quantity++;

	return 1;
}

void recibirRecursoPersonaje (char pj,t_list* listaCajas,t_list* listaPersonajes,t_list* listaPersonajesBloqueados,short socket){//recibe los recursos de un Pj y destruye la lista de recursos de ese pj, ya que a continuacion el pj va a morir SIEMPRE
	t_personaje *bufferPj;
	int cantRecursosPj;
	int i=0;
	bool _is_Personaje(t_personaje* pj2){
								if(pj2->id==pj)return true;
								return false;
							}
	bufferPj=list_find(listaPersonajes,(void*)_is_Personaje);
	cantRecursosPj=list_size(bufferPj->recursos);
	for(i=0;i<cantRecursosPj;i++){
		recibirRecursos(listaCajas,listaPersonajesBloqueados,*((char*)(list_get(bufferPj->recursos,i))),socket);
	}
	list_destroy(bufferPj->recursos);
}


int crearPersonaje(t_list* listaJugadoresActivos,int x,int y,char id){//IMPLEMENTAR QUE REVISE SI ESE PERSONAJE NO EXISTIA YA

	t_personaje *bufferPj;



	bool _is_Personaje(t_personaje* pj2){
								if(pj2->id==id)return true;
								return false;
							}

	if(list_find(listaJugadoresActivos,(void*)_is_Personaje)==NULL){


		bufferPj=malloc(sizeof(t_personaje));
		bufferPj->id=id;
		bufferPj->posx=x;
		bufferPj->posy=y;
		bufferPj->recursoEspera=' ';
	bufferPj->recursos=list_create();
	pthread_mutex_lock(&mutexCrearPersonaje);
	list_add(listaJugadoresActivos,bufferPj);
	pthread_mutex_unlock(&mutexCrearPersonaje);
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

int matarPersonaje(t_list* listaJugadoresActivos,t_list* listaJugadoresMuertos,t_list* listaJugadoresBloqueados,t_list* listaCajas,char id,short socket){
	t_personaje *bufferPj;
	bool _is_Personaje(t_personaje* pj2){
							if(pj2->id==id)return true;
							return false;
						}
		bufferPj=list_find(listaJugadoresActivos,(void*)_is_Personaje);
		if(bufferPj!=NULL){
			pthread_mutex_lock( &mutexMatarPersonaje);
			recibirRecursoPersonaje(id,listaCajas,listaJugadoresActivos,listaJugadoresBloqueados,socket);
			list_remove_by_condition(listaJugadoresActivos,(void*) _is_Personaje);
			//list_add(listaJugadoresMuertos,list_remove_by_condition(listaJugadoresActivos,(void*) _is_Personaje));
			pthread_mutex_unlock( &mutexMatarPersonaje);
			return 1;
		}
		else return -1;
}

int moverPersonaje(t_list* listaJugadoresActivos,int x,int y,char id){
	int i=0;
	t_personaje* buffer;

	buffer=malloc(sizeof(t_personaje));
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

