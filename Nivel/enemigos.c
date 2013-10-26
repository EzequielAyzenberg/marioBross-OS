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
#include <commons/collections/list.h>
#include <time.h>

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
				movArribaDer(i,buffer,rows,cols,ctrl,recorridoEnemigos);
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

void moverEnemigos(t_list* listaEnemigos,t_list listaCajas,t_list listaJugadoresActivos,coordenadas recorridoEnemigos[][4],int rows, int cols){
	int i=0;
	coordenadas* buffer;
	Caja* bufferCaja;

 //printf("LA cant de elem de la lista %d es %d\n",0,list_size(recorridoEnemigos[0]));
 int cantCajas;
	cantCajas=list_size(&listaCajas);
	int coorCajas[cantCajas];
	int cantEne;
	cantEne=list_size(listaEnemigos);
	for(i=0;i<cantCajas;i++){
		bufferCaja=(list_get(&listaCajas,i));
		coorCajas[i]=(*bufferCaja).posx*100+(*bufferCaja).posy;
	}
/*
	for(i=0;i<cantEne;i++){
		buffer=list_get(listaEnemigos,i);
		elegirMovimiento(i,buffer,coorCajas,cantCajas,rows,cols,cantEne,recorridoEnemigos);

		}
*/



	if (list_size(&listaJugadoresActivos)==0){


	for(i=0;i<cantEne;i++){

		if(recorridoEnemigos[i][3].posx==0 && recorridoEnemigos[i][3].posy==0){ //pregunta si no tiene movimientos para hacer el enemigo

			buffer=list_get(listaEnemigos,i);
			elegirMovimiento(i,buffer,rows,cols,cantEne,recorridoEnemigos[i]);
			int l=0;
			for(l=0;l<4;l++){
			//printf("Enemigo Nº%d posX:%d posY:%d\n",i,recorridoEnemigos[i][l].posx,recorridoEnemigos[i][l].posy);
			}
			//printf("El tamaño de la lista %d cuando sale es %d\n",i,list_size(recorridoEnemigos[i]));
		}

			buffer=list_get(listaEnemigos,i);
			int k=0;
			while(recorridoEnemigos[i][k].posx==0 && recorridoEnemigos[i][k].posy==0) k++;
			(*buffer).posx=recorridoEnemigos[i][k].posx;
			(*buffer).posy=recorridoEnemigos[i][k].posy;
			recorridoEnemigos[i][k].posx=0;
			recorridoEnemigos[i][k].posy=0;
			list_replace(listaEnemigos,i,buffer);

			//actualizarNivel(listaCajas,*listaEnemigos,listaJugadoresActivos,"nivel245");


		}


	}else{
		puts("no entra al if");//aca es cuando hay personajes
	}

}




