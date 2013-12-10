/*
 * enemigos.c
 *
 *  Created on: 22/10/2013
 *      Author: utnso
 */
#include <stdio.h>
#include <stdlib.h>
#include "cargador.h"
#include "enemigos.h"
#include "personajes.h"
#include <commons/collections/list.h>
#include <commons/log.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <theGRID/general.h>
#include "semaforos.h"

pthread_mutex_t mutexDibujar;
pthread_mutex_t mutexMatarPersonaje;
pthread_mutex_t mutexLog;


void cargarCoordenadasEnemigo(infoEnemigosThread info,int cantCajas,int coorCajas[cantCajas],int rows,int cols){
	//traigo la variable de control del otro ciclo (alla i y aca j) para sumarla y obtener un numero random distinto cada vez
	int i=0;
	int seed=0;//variable q sumo en cada iteracion para modificar la semilla de srand()

	int j;
	 j= (int)info.myinfo->idGoomba;

	coordenadas enemyPos;
	srand(time(NULL));
	seed=rand();
	srand(j*(99999));
	seed=seed+rand();
	srand(seed);
	enemyPos.posx=1+rand()%(cols);
	seed=rand();
	srand(seed);
	enemyPos.posy=1+rand()%(rows);

	for(i=0;i<cantCajas;i++){
		int flag=1;
		while(flag){
			if(enemyPos.posx*100+enemyPos.posy==coorCajas[i]){
				seed++;
				srand(time(NULL)+j+seed);
				enemyPos.posx=1+rand()%(cols);
				seed=rand();
				srand(seed);
				enemyPos.posy=1+rand()%(rows);
			}else (flag=0);
		}
	}

	info.myinfo->posx=enemyPos.posx;
	info.myinfo->posy=enemyPos.posy;



}

void crearEnemigos(infoEnemigosThread info,int rows,int cols){  //recibe el archivo de configuracion y los limites de pantalla

	int i=0;
	int cantCajas=0;
	cantCajas=list_size(info.listaCajas);
	int coorCajas[cantCajas];//este vector va a almacenar las coordenadas de cada caja
	Caja* buffer;

	for(i=0;i<cantCajas;i++){
		buffer=list_get(info.listaCajas,i);
		coorCajas[i]=(*buffer).posx*100+(*buffer).posy;
	}
	i=0;


		cargarCoordenadasEnemigo(info,cantCajas,coorCajas,rows,cols);//Carga las coordenadas del numero de enemigo indicado,necesita la cantidad de columnas y filas maximas


}

void movArribaIzq(int i,coordenadas* buffer,int rows,int cols,int* ctrl,coordenadas recorridoEnemigos[]){

	if(((*buffer).posx)-1>0 && (((*buffer).posy)-3>0)){ //comprueba que el movimiento que quiere hacer este dentro de los margenes del nivel

		recorridoEnemigos[0].posx=(*buffer).posx;
		recorridoEnemigos[0].posy=(*buffer).posy-1;

		recorridoEnemigos[1].posx=(*buffer).posx;
		recorridoEnemigos[1].posy=(*buffer).posy-2;

		recorridoEnemigos[2].posx=(*buffer).posx;
		recorridoEnemigos[2].posy=(*buffer).posy-3;

		recorridoEnemigos[3].posx=(*buffer).posx-1;
		recorridoEnemigos[3].posy=(*buffer).posy-3;
		}
		else{
			if (*ctrl<9) {
				*ctrl=*ctrl+1;
				movArribaDer(i,buffer,rows,cols,ctrl,recorridoEnemigos);
			}
		};
	}
void movArribaDer(int i,coordenadas* buffer,int rows,int cols,int* ctrl,coordenadas recorridoEnemigos[]){

	if(((*buffer).posx)+1<cols && (((*buffer).posy)-3>0)){ //comprueba que el movimiento que quiere hacer este dentro de los margenes del nivel

		recorridoEnemigos[0].posx=(*buffer).posx;
		recorridoEnemigos[0].posy=(*buffer).posy-1;

		recorridoEnemigos[1].posx=(*buffer).posx;
		recorridoEnemigos[1].posy=(*buffer).posy-2;

		recorridoEnemigos[2].posx=(*buffer).posx;
		recorridoEnemigos[2].posy=(*buffer).posy-3;

		recorridoEnemigos[3].posx=(*buffer).posx+1;
		recorridoEnemigos[3].posy=(*buffer).posy-3;
		}
		else{
			if (*ctrl<9) {
				*ctrl=*ctrl+1;
				movDerArriba(i,buffer,rows,cols,ctrl,recorridoEnemigos);
			}
		};
	}

void movDerArriba(int i,coordenadas* buffer,int rows,int cols,int* ctrl,coordenadas recorridoEnemigos[]){

	if(((*buffer).posx)+3<cols && (((*buffer).posy)-1>0)){ //comprueba que el movimiento que quiere hacer este dentro de los margenes del nivel

		recorridoEnemigos[0].posx=(*buffer).posx+1;
		recorridoEnemigos[0].posy=(*buffer).posy;

		recorridoEnemigos[1].posx=(*buffer).posx+2;
		recorridoEnemigos[1].posy=(*buffer).posy;

		recorridoEnemigos[2].posx=(*buffer).posx+3;
		recorridoEnemigos[2].posy=(*buffer).posy;

		recorridoEnemigos[3].posx=(*buffer).posx+3;
		recorridoEnemigos[3].posy=(*buffer).posy-1;
		}
		else{
			if (*ctrl<9) {
				*ctrl=*ctrl+1;
				movDerAbajo(i,buffer,rows,cols,ctrl,recorridoEnemigos);
			}
		};
	}
void movDerAbajo(int i,coordenadas* buffer,int rows,int cols,int* ctrl,coordenadas recorridoEnemigos[]){

	if(((*buffer).posx)+3<cols && (((*buffer).posy)+1<rows)){ //comprueba que el movimiento que quiere hacer este dentro de los margenes del nivel

		recorridoEnemigos[0].posx=(*buffer).posx+1;
		recorridoEnemigos[0].posy=(*buffer).posy;

		recorridoEnemigos[1].posx=(*buffer).posx+2;
		recorridoEnemigos[1].posy=(*buffer).posy;

		recorridoEnemigos[2].posx=(*buffer).posx+3;
		recorridoEnemigos[2].posy=(*buffer).posy;

		recorridoEnemigos[3].posx=(*buffer).posx+3;
		recorridoEnemigos[3].posy=(*buffer).posy+1;
		}
		else{
			if (*ctrl<9) {
				*ctrl=*ctrl+1;
				movAbajoDer(i,buffer,rows,cols,ctrl,recorridoEnemigos);
			}
		};
	}
void movAbajoDer(int i,coordenadas* buffer,int rows,int cols,int* ctrl,coordenadas recorridoEnemigos[]){

	if(((*buffer).posx)+1<cols && (((*buffer).posy)+3<rows)){ //comprueba que el movimiento que quiere hacer este dentro de los margenes del nivel

		recorridoEnemigos[0].posx=(*buffer).posx;
		recorridoEnemigos[0].posy=(*buffer).posy+1;

		recorridoEnemigos[1].posx=(*buffer).posx;
		recorridoEnemigos[1].posy=(*buffer).posy+2;

		recorridoEnemigos[2].posx=(*buffer).posx;
		recorridoEnemigos[2].posy=(*buffer).posy+3;

		recorridoEnemigos[3].posx=(*buffer).posx+1;
		recorridoEnemigos[3].posy=(*buffer).posy+3;
		}
		else{
			if (*ctrl<9) {
				*ctrl=*ctrl+1;
				movAbajoIzq(i,buffer,rows,cols,ctrl,recorridoEnemigos);
			}
		};
	}
void movAbajoIzq(int i,coordenadas* buffer,int rows,int cols,int* ctrl,coordenadas recorridoEnemigos[]){

	if(((*buffer).posx)-1>0 && (((*buffer).posy)+3<rows)){ //comprueba que el movimiento que quiere hacer este dentro de los margenes del nivel

		recorridoEnemigos[0].posx=(*buffer).posx;
		recorridoEnemigos[0].posy=(*buffer).posy+1;

		recorridoEnemigos[1].posx=(*buffer).posx;
		recorridoEnemigos[1].posy=(*buffer).posy+2;

		recorridoEnemigos[2].posx=(*buffer).posx;
		recorridoEnemigos[2].posy=(*buffer).posy+3;

		recorridoEnemigos[3].posx=(*buffer).posx-1;
		recorridoEnemigos[3].posy=(*buffer).posy+3;
		}
		else{
			if (*ctrl<9) {
				*ctrl=*ctrl+1;
				movIzqAbajo(i,buffer,rows,cols,ctrl,recorridoEnemigos);
			}
		};
	}
void movIzqAbajo(int i,coordenadas* buffer,int rows,int cols,int* ctrl,coordenadas recorridoEnemigos[]){

	if(((*buffer).posx)-3>0 && (((*buffer).posy)+1<rows)){ //comprueba que el movimiento que quiere hacer este dentro de los margenes del nivel

		recorridoEnemigos[0].posx=(*buffer).posx-1;
		recorridoEnemigos[0].posy=(*buffer).posy;

		recorridoEnemigos[1].posx=(*buffer).posx-2;
		recorridoEnemigos[1].posy=(*buffer).posy;

		recorridoEnemigos[2].posx=(*buffer).posx-3;
		recorridoEnemigos[2].posy=(*buffer).posy;

		recorridoEnemigos[3].posx=(*buffer).posx-3;
		recorridoEnemigos[3].posy=(*buffer).posy+1;

		}
		else{
			if (*ctrl<9) {
				*ctrl=*ctrl+1;
				movIzqArriba(i,buffer,rows,cols,ctrl,recorridoEnemigos);
			}
		};
	}
void movIzqArriba(int i,coordenadas* buffer,int rows,int cols,int* ctrl,coordenadas recorridoEnemigos[]){

	if(((*buffer).posx)-3>0 && (((*buffer).posy)-1>0)){ //comprueba que el movimiento que quiere hacer este dentro de los margenes del nivel

			recorridoEnemigos[0].posx=(*buffer).posx-1;
			recorridoEnemigos[0].posy=(*buffer).posy;

			recorridoEnemigos[1].posx=(*buffer).posx-2;
			recorridoEnemigos[1].posy=(*buffer).posy;

			recorridoEnemigos[2].posx=(*buffer).posx-3;
			recorridoEnemigos[2].posy=(*buffer).posy;

			recorridoEnemigos[3].posx=(*buffer).posx-3;
			recorridoEnemigos[3].posy=(*buffer).posy-1;

		}
		else{
			if (*ctrl<9) {
				*ctrl=*ctrl+1;
				movArribaIzq(i,buffer,rows,cols,ctrl,recorridoEnemigos);
			}
		};
}




void elegirMovimiento(int i,coordenadas* buffer,int rows,int cols,coordenadas recorridoEnemigos[]){

	int mov;
	int seed;
	srand(time(NULL));
	seed=rand();
	srand(i*(99999));
	seed=seed+rand();
	srand(seed);
	seed=rand();
	srand(seed);
	mov=1+rand()%8;
	int ctrl=0;//este int sirve para que el programa se de cuenta si no se puede mover el enemigo
	switch(mov){
	case 1:
		//puts("paso 1");
		movArribaIzq(i,buffer,rows,cols,&ctrl,recorridoEnemigos);
		break;
	case 2:
		//puts("paso 2");
		movArribaDer(i,buffer,rows,cols,&ctrl,recorridoEnemigos);
		break;
	case 3:
		//puts("paso 3");
		movDerArriba(i,buffer,rows,cols,&ctrl,recorridoEnemigos);
		break;
	case 4:
		//puts("paso 4");
		movDerAbajo(i,buffer,rows,cols,&ctrl,recorridoEnemigos);
		break;
	case 5:
		//puts("paso 5");
		movAbajoDer(i,buffer,rows,cols,&ctrl,recorridoEnemigos);
		break;
	case 6:
		//puts("paso 6");
		movAbajoIzq(i,buffer,rows,cols,&ctrl,recorridoEnemigos);
		break;
	case 7:
		//puts("paso 7");
		movIzqAbajo(i,buffer,rows,cols,&ctrl,recorridoEnemigos);
		break;
	case 8:
		//puts("paso 8");
		movIzqArriba(i,buffer,rows,cols,&ctrl,recorridoEnemigos);
		break;

	}
}

int calcularDistancia(coordenadas enemigo,t_personaje pj){
	int dis;
	dis=sqrt(pow(pj.posx-enemigo.posx,2)+pow(pj.posy-enemigo.posy,2));
	return dis;
}

int chequearSuperposicionCajas(coordenadas enemyPos,int cantCajas,int coorCajas[cantCajas],int ctrl){
	int super=0;
	int i=0;
	switch(ctrl){
	case 1: enemyPos.posx++; //hay que sumarle uno al eje X
	break;
	case 2: enemyPos.posx--; //hay que restarle uno al eje X
	break;
	case 3: enemyPos.posy++; //hay que sumarle uno al eje Y
	break;
	case 4: enemyPos.posy--; //hay que sumarle uno al eje Y
	break;
	};
	for (i=0;i<cantCajas;i++){
		if(enemyPos.posx*100+enemyPos.posy==coorCajas[i])super=1;

	}

	return super;
}

void jugadoresFocuseables(t_list* listaJugadoresActivos,t_list* listaJugadoresFocuseables,int coorCajas[],int cantCajas){

	int cantJugadoresActivos=list_size(listaJugadoresActivos);
	int i=0;
	int j=0;
	int flag=0;
	t_personaje* buffer;

	for(i=0;i<cantJugadoresActivos;i++){
		//buffer=(personaje*)malloc(sizeof(t_personaje));
		buffer=(t_personaje*)list_get(listaJugadoresActivos,i);
		if(!(buffer->posx==0&&buffer->posy==0)){
			j=0;
			for(j=0;j<cantCajas;j++){
				if(buffer->posx*100+buffer->posy==coorCajas[j])flag=1;
				//printf("iteracion N:%d\n",cantCajas);
			}
		}else flag=1;

	if(flag==0)list_add(listaJugadoresFocuseables,buffer);
	flag=0;
	//else free(buffer);
	}

}

void moverEnemigos(char* nombreNivel,coordenadas* myinfo,t_list* listaCajas,t_list* listaJugadoresActivos,t_list* listaJugadoresMuertos,coordenadas recorridoEnemigos[4],int rows, int cols,short *socketPlataforma,int* flagX){
	//t_log_level logDetalle=log_level_from_string("INFO");
	//t_log* logNivel=log_create("log","nivel1", 0, logDetalle);
	t_list* listaJugadoresFocuseables;
	listaJugadoresFocuseables=list_create();
	int i=0;
	int j=0;
	coordenadas* buffer;
	Caja* bufferCaja;
	t_personaje* bufferPersonaje;
	int cantCajas;

	cantCajas=list_size(listaCajas);

	int coorCajas[cantCajas];
	int cantEne;

	for(i=0;i<cantCajas;i++){
		bufferCaja=(list_get(listaCajas,i));
		coorCajas[i]=(*bufferCaja).posx*100+(*bufferCaja).posy;
	}
	jugadoresFocuseables(listaJugadoresActivos,listaJugadoresFocuseables,coorCajas,cantCajas);
	if (list_size(listaJugadoresFocuseables)==0){

			if(recorridoEnemigos[3].posx==0 && recorridoEnemigos[3].posy==0){ //pregunta si no tiene movimientos para hacer el enemigo

				elegirMovimiento((int)myinfo->idGoomba,myinfo,rows,cols,recorridoEnemigos);
				}

			int k=0;
			while(recorridoEnemigos[k].posx==0 && recorridoEnemigos[k].posy==0) k++;
			int l=0;
			for(l=0;l<cantCajas;l++){
				int flag=1;
				while(flag){
					if(recorridoEnemigos[k].posx*100+recorridoEnemigos[k].posy==coorCajas[l]){
							elegirMovimiento((int)myinfo->idGoomba,myinfo,rows,cols,recorridoEnemigos);
							k=0;
						}
						else{
							flag=0;
							}
								}
									}
			myinfo->posx=recorridoEnemigos[k].posx;
			myinfo->posy=recorridoEnemigos[k].posy;
			recorridoEnemigos[k].posx=0;
			recorridoEnemigos[k].posy=0;


			//actualizarNivel(listaCajas,*listaEnemigos,listaJugadoresActivos,"nivel245");





	}else{
		for(j=0;j<4;j++){
							recorridoEnemigos[j].posx=0;
							recorridoEnemigos[j].posy=0;
						}
		int cantPjActivos=list_size(listaJugadoresFocuseables);
		t_personaje posPjs[cantPjActivos];
		i=0;
		for(i=0;i<cantPjActivos;i++){
			bufferPersonaje=(t_personaje*)list_get(listaJugadoresFocuseables,i);
			if(bufferPersonaje!=NULL){
			posPjs[i].posx=(*bufferPersonaje).posx;
			posPjs[i].posy=(*bufferPersonaje).posy;
			posPjs[i].id=(*bufferPersonaje).id;
			}
		}
		i=0;
		int dist=0;
		int distMIN=999999;
		int numPj=1;


			j=0;
			for (j=0;j<cantPjActivos;j++){
				dist=calcularDistancia(*myinfo,posPjs[j]);//"dist" recibe el cuadrado de la distancia, para evitar el uso de sqrt
				//printf("%d\n",dist);

				if (dist<distMIN){
					distMIN=dist;

					numPj=j;
				}
			}
//printf("El pj es el numero %d\n",numPj);

//numPj=0;
			int yaMovio=0;//esta varaible esta para evitar que se mueva 2 veces en el mismo turno un enemigo
			if (*flagX==1){
					if(myinfo->posx==posPjs[numPj].posx||(myinfo->posx<posPjs[numPj].posx && chequearSuperposicionCajas(*myinfo,cantCajas,coorCajas,1))||(myinfo->posx>posPjs[numPj].posx && chequearSuperposicionCajas(*myinfo,cantCajas,coorCajas,2)))*flagX=0;//si ya estoy alineado con el eje X, voy a moverme en el eje Y
				};

			if (*flagX==0){
					if(myinfo->posy==posPjs[numPj].posy||(myinfo->posy<posPjs[numPj].posy && chequearSuperposicionCajas(*myinfo,cantCajas,coorCajas,3))||(myinfo->posy>posPjs[numPj].posy && chequearSuperposicionCajas(*myinfo,cantCajas,coorCajas,4)))*flagX=1;//si ya estoy alineado con el eje X, voy a moverme en el eje Y

					//if((*buffer).posy==posPjs[numPj].posy)flagX[i]=1;//si ya estoy alineado con el eje Y, voy a moverme en el eje X
					}


			if (*flagX==1){
								if(myinfo->posy==posPjs[numPj].posy && ((myinfo->posx<posPjs[numPj].posx && chequearSuperposicionCajas(*myinfo,cantCajas,coorCajas,1))||(myinfo->posx>posPjs[numPj].posx && chequearSuperposicionCajas(*myinfo,cantCajas,coorCajas,2)))){
									if (rand()%2==1)myinfo->posy++;
									else myinfo->posy--;
									yaMovio=1;
								}
							}


			if (*flagX==0){
									if(myinfo->posx==posPjs[numPj].posx && ((myinfo->posy<posPjs[numPj].posy && chequearSuperposicionCajas(*myinfo,cantCajas,coorCajas,3))||(myinfo->posy>posPjs[numPj].posy && chequearSuperposicionCajas(*myinfo,cantCajas,coorCajas,4)))){
										if (rand()%2==1)myinfo->posx++;
										else myinfo->posx--;
										yaMovio=1;
									}
							}


			if (!yaMovio){
			if (*flagX==1){
					if(myinfo->posx<posPjs[numPj].posx)myinfo->posx=myinfo->posx+1;

					else myinfo->posx=myinfo->posx-1;

					*flagX=0;
				}else{
					if(myinfo->posy<posPjs[numPj].posy)myinfo->posy=myinfo->posy+1;

					else myinfo->posy=myinfo->posy-1;


					*flagX=1;
				}
			}
			if(myinfo->posx==posPjs[numPj].posx && myinfo->posy==posPjs[numPj].posy){
				pthread_mutex_lock( &mutexMatarPersonaje);
				bool _is_Personaje(t_personaje* pj2){
											if(pj2->id==posPjs[numPj].id)return true;
											return false;
										}

				if((list_find(listaJugadoresFocuseables,(void*)_is_Personaje))!=NULL){
				//printf("El jugador %c ha sido pisado por un goomba",posPjs[numPj].id);
				char* bufferMsg=(char*)malloc(50);

				strcpy(bufferMsg,"El jugador");

				strcat(bufferMsg,"   ha sido pisado por un goomba");

				bufferMsg[11]=posPjs[numPj].id;

				/*
				pthread_mutex_lock( &mutexLog);
				loguearInfo(bufferMsg);

				pthread_mutex_unlock( &mutexLog);
*/








				char *nombreLog;
				nombreLog=(char*)malloc(40);
				strcpy(nombreLog,nombreNivel);
				strcat(nombreLog,"Log");
				pthread_mutex_lock( &mutexLog);
				t_log_level logDetalle=log_level_from_string("INFO");
				t_log* logNivel=log_create(nombreLog,nombreNivel, 0, logDetalle);
				log_info(logNivel,bufferMsg);
				log_destroy(logNivel);
				pthread_mutex_unlock( &mutexLog);

				free(bufferMsg);
				free (nombreLog);
				bool _is_Personaje(t_personaje* pj){
					if(pj->id==posPjs[numPj].id)return true;
					return false;
				}

				recibirRecursoPersonaje(posPjs[numPj].id,listaCajas,listaJugadoresActivos);
				free(list_remove_by_condition(listaJugadoresActivos,(void*) _is_Personaje));

				//list_add(listaJugadoresMuertos,list_remove_by_condition(listaJugadoresActivos,(void*) _is_Personaje));//CUIDADO CUANDO DOS ENEMIGOS MATAN A LA VEZ AL MISMO SEGMENTATION FAULT PONER SEMAFORO
				sendAnswer(8,0,' ',posPjs[numPj].id,*socketPlataforma);
				//printf("%d",*socketPlataforma);
				pthread_mutex_unlock( &mutexMatarPersonaje);

				}
				else pthread_mutex_unlock( &mutexMatarPersonaje);
			}
			//focusear(i,buffer,posPjs[numPj]);//Search and destroy


	}
	list_destroy(listaJugadoresFocuseables);

	//log_destroy(logNivel);
}


void controlEnemigos(infoEnemigosThread* info){
//printf("mi direccion de memoria es %d\n",(int)info->myinfo);

int flagX=0;

coordenadas recorridoEnemigos [4];

		int j=0;

			for(j=0;j<4;j++){
				recorridoEnemigos[j].posx=0;
				recorridoEnemigos[j].posy=0;
			}

//printf("Estoy en hilo %d\n",(int)info->myinfo->idGoomba);
	crearEnemigos(*info,(*info).rows,(*info).cols);
	pthread_mutex_lock( &mutexDibujar);
	//printf("%d\n",info->myinfo->posx);
	//printf("%d\n",info->myinfo->posy);
	actualizarNivel(*info->listaCajas,*info->listaEnemigos,*info->listaJugadoresActivos,info->nombreNivel);
	pthread_mutex_unlock( &mutexDibujar);
	//actualizarNivel(*(*info).listaCajas,*(*info).listaEnemigos,*(*info).listaJugadoresActivos,(*info).nombreNivel);
	//printf("%d\n",list_size(info->listaEnemigos));
	while(1){


		usleep((*info).sleepEnemigos*1000);



		moverEnemigos((*info).nombreNivel,(*info).myinfo,(*info).listaCajas,(*info).listaJugadoresActivos,(*info).listaJugadoresMuertos,recorridoEnemigos,(*info).rows,(*info).cols,info->socket,&flagX);

		pthread_mutex_lock( &mutexDibujar);

		actualizarNivel(*(*info).listaCajas,*(*info).listaEnemigos,*(*info).listaJugadoresActivos,(*info).nombreNivel);

		pthread_mutex_unlock( &mutexDibujar);


	}


}

