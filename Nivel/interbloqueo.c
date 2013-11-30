/*
 * interbloqueo.c
 *
 *  Created on: 23/11/2013
 *      Author: utnso
 */
#include <stdio.h>
#include <stdlib.h>
#include "interbloqueo.h"
#include <commons/collections/list.h>
#include "cargador.h"
#include "personajes.h"
#include "interbloqueo.h"
#include <string.h>


int posicionIndice(char recurso,char indice[]){
	while(1){
		int i=0;
		if(indice[i]==recurso)return i;
		i++;
	}
return -1;//sino pongo esto, el compilador tira warning
}

int tengoRecursos(int work[],int espera[],int cantJugadores){
	int i=0;
	int j=0;
	int tengo=1;
	for (i=0;i<cantJugadores;i++){
		if (espera[i]>work[i])tengo=-1;
		}
	return tengo;
}
void detectarInterbloqueo(infoInterbloqueo *info){
	int disponible[list_size(info->listaRecursos)];//este array va a almacenar la cantidad de recursos disponibles
		int work[list_size(info->listaRecursos)];
		char indice[list_size(info->listaRecursos)];//indica que tipo de recurso hay en cada pos del array disponible
		int asignacion[list_size(info->listaJugadores)][list_size(info->listaRecursos)];
		int espera[list_size(info->listaJugadores)][list_size(info->listaRecursos)];
		int finish[list_size(info->listaJugadores)];


		int i=0;
		int j=0;
		int k=0;

		for (i=0;i<list_size(info->listaRecursos);i++){
			disponible[i]=0;
			indice[i]=' ';
			for(j=0;j<list_size(info->listaJugadores);j++){
				asignacion[j][i]=0;
				espera[j][i]=0;
			}
		}



	Caja *bufferCaja;
	t_personaje *bufferPersonaje;
	int cantRecPj=0;//cantidad de recursos que tiene un personaje
	char *recurso; //un recurso que tiene el pj

	int interbloqueo;//esta variable es un "boolean que ams adelante sirve para saber si hay o no interbloqueo
	interbloqueo=0;
		i=0;
		j=0;
		for(i=0;i<list_size(info->listaRecursos);i++){
			bufferCaja=list_get(info->listaRecursos,i);
			indice[i]=bufferCaja->id;
			disponible[i]=bufferCaja->quantity;
			work[i]=disponible[i];
			}
		i=0;
		for(i=0;i<list_size(info->listaJugadores);i++){
		//	loguearInfo("analizando");
			bufferPersonaje=list_get(info->listaJugadores,i);

			cantRecPj=list_size(bufferPersonaje->recursos);
			if(bufferPersonaje->recursoEspera!=' ')printf("esta esperando un !%c!",bufferPersonaje->recursoEspera);
			if(bufferPersonaje->recursoEspera!=' '){espera[i][posicionIndice(bufferPersonaje->recursoEspera,indice)]++;
			loguearInfo(bufferPersonaje->id);
			loguearInfo("espera");
			loguearInfo(bufferPersonaje->recursoEspera);
			}
			for(j=0;j<cantRecPj;j++){
			recurso=list_get(bufferPersonaje->recursos,j);
			asignacion[i][posicionIndice(*recurso,indice)]++;
				}

			}



		i=0;
		j=0;
		char asd[200];
		for (i=0;i<list_size(info->listaJugadores);i++){
			bufferPersonaje=list_get(info->listaJugadores,i);
			if(list_size(bufferPersonaje->recursos)==0){//siempre 0 CORREGIR
				itoa(list_size(bufferPersonaje->recursos),asd);
				strcat(asd,":recursos asignados");
				loguearInfo(asd);
				finish[i]=1;
			}
			else finish[i]=0;
			}
		for(k=0;k<list_size(info->listaJugadores);k++){
			i=0;
			j=0;
			for(i=0;i<(list_size(info->listaJugadores));i++){
				if (finish[i]==0){
					if(tengoRecursos(work,espera[i],list_size(info->listaJugadores))){
						//finish[i]=1;
						for(j=0;j<list_size(info->listaRecursos);j++){
							work[j]=work[j]+asignacion[i][j];
							}


						}

					}

				}



			}
		i=0;
		char msg[200];
		char *itoas=malloc(1);
		char *itoas2=malloc(1);
		char *itoas3=malloc(1);
		for(i=0;i<list_size(info->listaJugadores);i++){
			itoa(finish[i],itoas);
			itoa(i,itoas2);
			bufferPersonaje=list_get(info->listaJugadores,i);
			strcpy(itoas3,&bufferPersonaje->recursoEspera);
			strcpy(msg,"el jugador numero:");
			strcat(msg,itoas2);
			strcat(msg," finalizo:");
			strcat(msg,itoas);
			strcat(msg," y esta esperando:");
			strcat(msg,itoas3);
			loguearInfo(msg);
		if(finish[i]==0){
			interbloqueo=1;
			bufferPersonaje=list_get(info->listaJugadores,i);
			printf("matamos a %c",bufferPersonaje->id);
			matarPersonaje(info->listaJugadores,info->listaJugadoresMuertos,info->listaRecursos,bufferPersonaje->id);

			break;
						}

			}

}
void controlInterbloqueo(infoInterbloqueo *info){









	while(1){









	usleep(info->tiempoEspera*1000);
	detectarInterbloqueo(info);
	}
puts("finalizo el hilo");
}
