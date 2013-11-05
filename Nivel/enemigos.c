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

void cargarCoordenadasEnemigo(int j,t_list* listaEnemigos,int cantCajas,int coorCajas[cantCajas],int rows,int cols){
	//traigo la variable de control del otro ciclo (alla i y aca j) para sumarla y obtener un numero random distinto cada vez
	int i=0;
	int seed=0;//variable q sumo en cada iteracion para modificar la semilla de srand()

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

	coordenadas* buffer;
	buffer=(coordenadas*)malloc(sizeof(coordenadas));
	*buffer=enemyPos;

	list_add(listaEnemigos,buffer);

};

void crearEnemigos(nivelConfig config,t_list* listaEnemigos,int rows,int cols){  //recibe el archivo de configuracion y los limites de pantalla

	int i=0;

	int cantEne=0;
	int cantCajas=0;
	*listaEnemigos=*list_create();

	cantEne=config.enemigos;
	cantCajas=list_size(&config.listaCajas);
	int coorCajas[cantCajas];//este vector va a almacenar las coordenadas de cada caja
	Caja* buffer;

	for(i=0;i<cantCajas;i++){
		buffer=list_get(&config.listaCajas,i);
		coorCajas[i]=(*buffer).posx*100+(*buffer).posy;
	}
	i=0;

	for(i=0;i<cantEne;i++){
		cargarCoordenadasEnemigo(i,listaEnemigos,cantCajas,coorCajas,rows,cols);//Carga las coordenadas del numero de enemigo indicado,necesita la cantidad de columnas y filas maximas
		}

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




void elegirMovimiento(int i,coordenadas* buffer,int rows,int cols,int cantEne,coordenadas recorridoEnemigos[]){
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

int calcularDistancia(coordenadas enemigo,personaje pj){
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



void moverEnemigos(t_list* listaEnemigos,t_list* listaCajas,t_list* listaJugadoresActivos,t_list* listaJugadoresMuertos,coordenadas recorridoEnemigos[][4],int rows, int cols,int flagX[]){

	int i=0;
	int j=0;
	coordenadas* buffer;
	Caja* bufferCaja;
	personaje* bufferPersonaje;
	int cantCajas;

	cantCajas=list_size(listaCajas);

	int coorCajas[cantCajas];
	int cantEne;
	cantEne=list_size(listaEnemigos);
	for(i=0;i<cantCajas;i++){
		bufferCaja=(list_get(listaCajas,i));
		coorCajas[i]=(*bufferCaja).posx*100+(*bufferCaja).posy;
	}

	if (list_size(listaJugadoresActivos)==0){
		for(i=0;i<cantEne;i++){
			if(recorridoEnemigos[i][3].posx==0 && recorridoEnemigos[i][3].posy==0){ //pregunta si no tiene movimientos para hacer el enemigo
				buffer=list_get(listaEnemigos,i);
				elegirMovimiento(i,buffer,rows,cols,cantEne,recorridoEnemigos[i]);
				}
			buffer=list_get(listaEnemigos,i);
			int k=0;
			while(recorridoEnemigos[i][k].posx==0 && recorridoEnemigos[i][k].posy==0) k++;
			int l=0;
			for(l=0;l<cantCajas;l++){
				int flag=1;
				while(flag){
					if(recorridoEnemigos[i][k].posx*100+recorridoEnemigos[i][k].posy==coorCajas[l]){
							elegirMovimiento(l,buffer,rows,cols,cantEne,recorridoEnemigos[i]);
							k=0;
						}
						else{
							flag=0;
							}
								}
									}
			(*buffer).posx=recorridoEnemigos[i][k].posx;
			(*buffer).posy=recorridoEnemigos[i][k].posy;
			recorridoEnemigos[i][k].posx=0;
			recorridoEnemigos[i][k].posy=0;
			list_replace(listaEnemigos,i,buffer);

			//actualizarNivel(listaCajas,*listaEnemigos,listaJugadoresActivos,"nivel245");


		}


	}else{
		int cantPjActivos=list_size(listaJugadoresActivos);
		personaje posPjs[cantPjActivos];
		i=0;
		for(i=0;i<cantPjActivos;i++){
			bufferPersonaje=list_get(listaJugadoresActivos,i);
			posPjs[i].posx=(*bufferPersonaje).posx;
			posPjs[i].posy=(*bufferPersonaje).posy;
			posPjs[i].id=(*bufferPersonaje).id;
		}
		i=0;
		int dist=0;
		int distMIN=999999;
		int numPj;
		for (i=0;i<cantEne;i++){
			buffer=list_get(listaEnemigos,i);
			j=0;
			for (j=0;j<cantPjActivos;j++){
				dist=calcularDistancia(*buffer,posPjs[j]);//"dist" recibe el cuadrado de la distancia, para evitar el uso de sqrt
			//	printf("%d\n",dist);

				if (dist<distMIN){
					distMIN=dist;
					numPj=j;
				}
			}
//printf("%d\n",numPj);


			if (flagX[i]==1){
					if((*buffer).posx==posPjs[numPj].posx||((*buffer).posx<posPjs[numPj].posx && chequearSuperposicionCajas((*buffer),cantCajas,coorCajas,1))||((*buffer).posx>posPjs[numPj].posx && chequearSuperposicionCajas((*buffer),cantCajas,coorCajas,2)))flagX[i]=0;//si ya estoy alineado con el eje X, voy a moverme en el eje Y
				};

			if (flagX[i]==0){
					if((*buffer).posy==posPjs[numPj].posy||((*buffer).posy<posPjs[numPj].posy && chequearSuperposicionCajas((*buffer),cantCajas,coorCajas,3))||((*buffer).posy>posPjs[numPj].posy && chequearSuperposicionCajas((*buffer),cantCajas,coorCajas,4)))flagX[i]=0;//si ya estoy alineado con el eje X, voy a moverme en el eje Y

					//if((*buffer).posy==posPjs[numPj].posy)flagX[i]=1;//si ya estoy alineado con el eje Y, voy a moverme en el eje X
					}

			if (flagX[i]==1){
					if((*buffer).posx<posPjs[numPj].posx)(*buffer).posx=(*buffer).posx+1;
					else (*buffer).posx=(*buffer).posx-1;
					flagX[i]=0;
				}else{
					if((*buffer).posy<posPjs[numPj].posy)(*buffer).posy=(*buffer).posy+1;
					else (*buffer).posy=(*buffer).posy-1;
					flagX[i]=1;
				}
			if((*buffer).posx==posPjs[numPj].posx && (*buffer).posy==posPjs[numPj].posy){
				printf("El jugador %c ha sido pisado por un goomba",posPjs[numPj].id);
				char* bufferMsg=(char*)malloc(50);
				strcpy(bufferMsg,"El jugador");

				strcat(bufferMsg,"   ha sido pisado por un goomba");
				bufferMsg[11]=posPjs[numPj].id;
				t_log_level logDetalle=log_level_from_string("INFO");
				t_log* logNivel=log_create("log","nivel1", 0, logDetalle);
				log_info(logNivel,bufferMsg);
				free(bufferMsg);

				list_add(listaJugadoresMuertos,list_remove(listaJugadoresActivos,numPj));
			}

			//focusear(i,buffer,posPjs[numPj]);//Search and destroy
			//list_replace(listaEnemigos,i,buffer);
		}

	}


}


void controlEnemigos(infoEnemigosThread* info){

	int flagX[(*info).cantEne];
	int l=0;
	for(l=0;l<(*info).cantEne;l++){
		flagX[l]=0;
	}

coordenadas recorridoEnemigos [(*info).cantEne][4];
{
		int i=0,j=0;
		for (i=0;i<(*info).cantEne;i++){
			for(j=0;j<4;j++){
				recorridoEnemigos[i][j].posx=0;
				recorridoEnemigos[i][j].posy=0;
			}
		}
	}
	while(1){


		usleep((*info).sleepEnemigos*1000);



		moverEnemigos((*info).listaEnemigos,(*info).listaCajas,(*info).listaJugadoresActivos,(*info).listaJugadoresMuertos,recorridoEnemigos,(*info).rows,(*info).cols,flagX);

		actualizarNivel(*(*info).listaCajas,*(*info).listaEnemigos,*(*info).listaJugadoresActivos,(*info).nombreNivel);

}


}

