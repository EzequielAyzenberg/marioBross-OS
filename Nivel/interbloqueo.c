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
	int i=0;
	while(1){


		if(indice[i]==recurso){

			return i;
		}
		i++;
	}
return -1;//sino pongo esto, el compilador tira warning
}

int tengoRecursos(int work[],int espera[],int cantCajas){
	int i=0;
	int j=0;
	int tengo=1;
	for (i=0;i<cantCajas;i++){
		if (espera[i]>work[i])tengo=0;
		}
	return tengo;
}
void detectarInterbloqueo(infoInterbloqueo *info){

	{			// LOGGEO!!!!
		int i=0;
		char msg[512],valor[16];
		t_personaje *bufferPersonaje;
		char *recursoPj;
		strcpy(msg,"Inicio del log de personajes (ID/RS/[-RA-]):");
		void _cargarLogRec(char*rc){
			valor[0]=*rc;
			valor[1]='-';
			valor[2]='\0';
			strcat(msg,valor);
		}
		void _cargarLogJug(t_personaje*pj){
			strcat(msg,"--(");
			itoa(pj->id,valor);
			strcat(msg,valor);
			strcat(msg,"/");
			valor[0]=pj->recursoEspera;
			valor[1]='/';
			valor[2]='[';
			valor[3]='-';
			valor[4]='\0';
			strcat(msg,valor);
			list_iterate(pj->recursos,(void*)_cargarLogRec);
			strcat(msg,"])--");
		}
		list_iterate(info->listaJugadores,(void*)_cargarLogJug);

		loguearInfo(msg);
	}



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
			loguearInfo("analizando");
			cantRecPj=list_size(bufferPersonaje->recursos);
			//if(bufferPersonaje->recursoEspera!=' ')printf("esta esperando un !%c!",bufferPersonaje->recursoEspera);
			if(bufferPersonaje->recursoEspera!=' '){espera[i][posicionIndice(bufferPersonaje->recursoEspera,indice)]++;
			loguearInfo("analizando2");
			//loguearInfo(bufferPersonaje->id);
			//loguearInfo("espera");
			//loguearInfo(bufferPersonaje->recursoEspera);
			}
			loguearInfo("analizando3");
			for(j=0;j<cantRecPj;j++){
			recurso=list_get(bufferPersonaje->recursos,j);
			asignacion[i][posicionIndice(*recurso,indice)]++;
				}

			}

		{			// LOGGEO!!!!
				int i=0;
				char msg[512],valor[16];
				t_personaje *bufferPersonaje;
				char *recursoPj;
				strcpy(msg,"HELL YEAH Inicio del log de personajes (ID/RS/[-RA-]):");
				void _cargarLogRec(char*rc){
					valor[0]=*rc;
					valor[1]='-';
					valor[2]='\0';
					strcat(msg,valor);
				}
				void _cargarLogJug(t_personaje*pj){
					strcat(msg,"--(");
					itoa(pj->id,valor);
					strcat(msg,valor);
					strcat(msg,"/");
					valor[0]=pj->recursoEspera;
					valor[1]='/';
					valor[2]='[';
					valor[3]='-';
					valor[4]='\0';
					strcat(msg,valor);
					list_iterate(pj->recursos,(void*)_cargarLogRec);
					strcat(msg,"])--");
				}
				list_iterate(info->listaJugadores,(void*)_cargarLogJug);

				loguearInfo(msg);
			}


		i=0;
		j=0;
		char asd[256];
		for (i=0;i<list_size(info->listaJugadores);i++){
			bufferPersonaje=(t_personaje*)list_get(info->listaJugadores,i);
			if(list_is_empty(bufferPersonaje->recursos)){//siempre 0 CORREGIR
				itoa(list_size(bufferPersonaje->recursos),asd);
				strcat(asd,":recursos asignados");
				loguearInfo(asd);
				finish[i]=1;
			}
			else {
				finish[i]=0;
				}
			}
		for(k=0;k<list_size(info->listaJugadores);k++){
			i=0;
			j=0;
			for(i=0;i<(list_size(info->listaJugadores));i++){
				if (finish[i]==0){
					if(tengoRecursos(work,espera[i],list_size(info->listaRecursos))){
						finish[i]=1;
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
			sendAnswer(8,0,' ',bufferPersonaje->id,*info->socket);
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
