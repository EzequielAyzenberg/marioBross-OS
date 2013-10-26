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

cargarCoordenadasEnemigo(int j,t_list* listaEnemigos,int cantCajas,int coorCajas[cantCajas],int rows,int cols){
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

crearEnemigos(nivelConfig config,t_list* listaEnemigos,int rows,int cols){  //recibe el archivo de configuracion y los limites de pantalla

	int i=0;
	int j=0;
	int cantEne=0;
	int cantCajas=0;
	*listaEnemigos=*list_create();
	coordenadas enemyPos;
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
/*
int comprobarSuperposicion(coordenadas* buffer, int coorCajas[],int cantCajas,int mov){
	int i=0;
	int flag=1;
	for(i=0;i<cantCajas;i++){
		if((*buffer).posx*100+(*buffer).posy==coorCajas[i]){
			flag=0;
		}
	}
	return 1;
}
*/
movArribaIzq(int i,coordenadas* buffer, int coorCajas[],int cantCajas,int rows,int cols,int* ctrl,t_list* recorridoEnemigos){

	if(((*buffer).posx)-1>=0 && (((*buffer).posy)-3>=0)){ //comprueba que el movimiento que quiere hacer este dentro de los margenes del nivel
			*recorridoEnemigos=*list_create();
			coordenadas* movimientos;
			movimientos=(coordenadas*)malloc(sizeof(coordenadas));
			(*movimientos).posx=(*buffer).posx;
			(*movimientos).posy=(*buffer).posy-1;
			list_add(recorridoEnemigos,movimientos);
			movimientos=(coordenadas*)malloc(sizeof(coordenadas));
			(*movimientos).posx=(*buffer).posx;
			(*movimientos).posy=(*buffer).posy-2;
			list_add(recorridoEnemigos,movimientos);
			movimientos=(coordenadas*)malloc(sizeof(coordenadas));
			(*movimientos).posx=(*buffer).posx;
			(*movimientos).posy=(*buffer).posy-3;
			list_add(recorridoEnemigos,movimientos);
			movimientos=(coordenadas*)malloc(sizeof(coordenadas));
			(*movimientos).posx=(*buffer).posx-1;
			(*movimientos).posy=(*buffer).posy-3;
			list_add(recorridoEnemigos,movimientos);
		}
		else{
			if (*ctrl<9) {
				*ctrl=*ctrl+1;
				movArribaDer(i,buffer,coorCajas,cantCajas,rows,cols,ctrl,recorridoEnemigos);
			}
		};
	}
movArribaDer(int i,coordenadas* buffer, int coorCajas[],int cantCajas,int rows,int cols,int* ctrl,t_list* recorridoEnemigos){

	if(((*buffer).posx)-1>=0 && (((*buffer).posy)-3>=0)){ //comprueba que el movimiento que quiere hacer este dentro de los margenes del nivel
			*recorridoEnemigos=*list_create();
			coordenadas* movimientos;
			movimientos=(coordenadas*)malloc(sizeof(coordenadas));
			(*movimientos).posx=(*buffer).posx;
			(*movimientos).posy=(*buffer).posy-1;
			list_add(recorridoEnemigos,movimientos);
			movimientos=(coordenadas*)malloc(sizeof(coordenadas));
			(*movimientos).posx=(*buffer).posx;
			(*movimientos).posy=(*buffer).posy-2;
			list_add(recorridoEnemigos,movimientos);
			movimientos=(coordenadas*)malloc(sizeof(coordenadas));
			(*movimientos).posx=(*buffer).posx;
			(*movimientos).posy=(*buffer).posy-3;
			list_add(recorridoEnemigos,movimientos);
			movimientos=(coordenadas*)malloc(sizeof(coordenadas));
			(*movimientos).posx=(*buffer).posx-1;
			(*movimientos).posy=(*buffer).posy-3;
			list_add(recorridoEnemigos,movimientos);
		}
		else{
			if (*ctrl<9) {
				*ctrl=*ctrl+1;
				movArribaDer(i,buffer,coorCajas,cantCajas,rows,cols,ctrl,recorridoEnemigos);
			}
		};
	}

movDerArriba(int i,coordenadas* buffer, int coorCajas[],int cantCajas,int rows,int cols,int* ctrl,t_list* recorridoEnemigos){

	if(((*buffer).posx)-1>=0 && (((*buffer).posy)-3>=0)){ //comprueba que el movimiento que quiere hacer este dentro de los margenes del nivel
			*recorridoEnemigos=*list_create();
			coordenadas* movimientos;
			movimientos=(coordenadas*)malloc(sizeof(coordenadas));
			(*movimientos).posx=(*buffer).posx;
			(*movimientos).posy=(*buffer).posy-1;
			list_add(recorridoEnemigos,movimientos);
			movimientos=(coordenadas*)malloc(sizeof(coordenadas));
			(*movimientos).posx=(*buffer).posx;
			(*movimientos).posy=(*buffer).posy-2;
			list_add(recorridoEnemigos,movimientos);
			movimientos=(coordenadas*)malloc(sizeof(coordenadas));
			(*movimientos).posx=(*buffer).posx;
			(*movimientos).posy=(*buffer).posy-3;
			list_add(recorridoEnemigos,movimientos);
			movimientos=(coordenadas*)malloc(sizeof(coordenadas));
			(*movimientos).posx=(*buffer).posx-1;
			(*movimientos).posy=(*buffer).posy-3;
			list_add(recorridoEnemigos,movimientos);
		}
		else{
			if (*ctrl<9) {
				*ctrl=*ctrl+1;
				movArribaDer(i,buffer,coorCajas,cantCajas,rows,cols,ctrl,recorridoEnemigos);
			}
		};
	}
movDerAbajo(int i,coordenadas* buffer, int coorCajas[],int cantCajas,int rows,int cols,int* ctrl,t_list* recorridoEnemigos){

	if(((*buffer).posx)-1>=0 && (((*buffer).posy)-3>=0)){ //comprueba que el movimiento que quiere hacer este dentro de los margenes del nivel
			*recorridoEnemigos=*list_create();
			coordenadas* movimientos;
			movimientos=(coordenadas*)malloc(sizeof(coordenadas));
			(*movimientos).posx=(*buffer).posx;
			(*movimientos).posy=(*buffer).posy-1;
			list_add(recorridoEnemigos,movimientos);
			movimientos=(coordenadas*)malloc(sizeof(coordenadas));
			(*movimientos).posx=(*buffer).posx;
			(*movimientos).posy=(*buffer).posy-2;
			list_add(recorridoEnemigos,movimientos);
			movimientos=(coordenadas*)malloc(sizeof(coordenadas));
			(*movimientos).posx=(*buffer).posx;
			(*movimientos).posy=(*buffer).posy-3;
			list_add(recorridoEnemigos,movimientos);
			movimientos=(coordenadas*)malloc(sizeof(coordenadas));
			(*movimientos).posx=(*buffer).posx-1;
			(*movimientos).posy=(*buffer).posy-3;
			list_add(recorridoEnemigos,movimientos);
		}
		else{
			if (*ctrl<9) {
				*ctrl=*ctrl+1;
				movArribaDer(i,buffer,coorCajas,cantCajas,rows,cols,ctrl,recorridoEnemigos);
			}
		};
	}
movAbajoDer(int i,coordenadas* buffer, int coorCajas[],int cantCajas,int rows,int cols,int* ctrl,t_list* recorridoEnemigos){

	if(((*buffer).posx)-1>=0 && (((*buffer).posy)-3>=0)){ //comprueba que el movimiento que quiere hacer este dentro de los margenes del nivel
			*recorridoEnemigos=*list_create();
			coordenadas* movimientos;
			movimientos=(coordenadas*)malloc(sizeof(coordenadas));
			(*movimientos).posx=(*buffer).posx;
			(*movimientos).posy=(*buffer).posy-1;
			list_add(recorridoEnemigos,movimientos);
			movimientos=(coordenadas*)malloc(sizeof(coordenadas));
			(*movimientos).posx=(*buffer).posx;
			(*movimientos).posy=(*buffer).posy-2;
			list_add(recorridoEnemigos,movimientos);
			movimientos=(coordenadas*)malloc(sizeof(coordenadas));
			(*movimientos).posx=(*buffer).posx;
			(*movimientos).posy=(*buffer).posy-3;
			list_add(recorridoEnemigos,movimientos);
			movimientos=(coordenadas*)malloc(sizeof(coordenadas));
			(*movimientos).posx=(*buffer).posx-1;
			(*movimientos).posy=(*buffer).posy-3;
			list_add(recorridoEnemigos,movimientos);
		}
		else{
			if (*ctrl<9) {
				*ctrl=*ctrl+1;
				movArribaDer(i,buffer,coorCajas,cantCajas,rows,cols,ctrl,recorridoEnemigos);
			}
		};
	}
movAbajoIzq(int i,coordenadas* buffer, int coorCajas[],int cantCajas,int rows,int cols,int* ctrl,t_list* recorridoEnemigos){

	if(((*buffer).posx)-1>=0 && (((*buffer).posy)-3>=0)){ //comprueba que el movimiento que quiere hacer este dentro de los margenes del nivel
			*recorridoEnemigos=*list_create();
			coordenadas* movimientos;
			movimientos=(coordenadas*)malloc(sizeof(coordenadas));
			(*movimientos).posx=(*buffer).posx;
			(*movimientos).posy=(*buffer).posy-1;
			list_add(recorridoEnemigos,movimientos);
			movimientos=(coordenadas*)malloc(sizeof(coordenadas));
			(*movimientos).posx=(*buffer).posx;
			(*movimientos).posy=(*buffer).posy-2;
			list_add(recorridoEnemigos,movimientos);
			movimientos=(coordenadas*)malloc(sizeof(coordenadas));
			(*movimientos).posx=(*buffer).posx;
			(*movimientos).posy=(*buffer).posy-3;
			list_add(recorridoEnemigos,movimientos);
			movimientos=(coordenadas*)malloc(sizeof(coordenadas));
			(*movimientos).posx=(*buffer).posx-1;
			(*movimientos).posy=(*buffer).posy-3;
			list_add(recorridoEnemigos,movimientos);
		}
		else{
			if (*ctrl<9) {
				*ctrl=*ctrl+1;
				movArribaDer(i,buffer,coorCajas,cantCajas,rows,cols,ctrl,recorridoEnemigos);
			}
		};
	}
movIzqAbajo(int i,coordenadas* buffer, int coorCajas[],int cantCajas,int rows,int cols,int* ctrl,t_list* recorridoEnemigos){

	if(((*buffer).posx)-1>=0 && (((*buffer).posy)-3>=0)){ //comprueba que el movimiento que quiere hacer este dentro de los margenes del nivel
			*recorridoEnemigos=*list_create();
			coordenadas* movimientos;
			movimientos=(coordenadas*)malloc(sizeof(coordenadas));
			(*movimientos).posx=(*buffer).posx;
			(*movimientos).posy=(*buffer).posy-1;
			list_add(recorridoEnemigos,movimientos);
			movimientos=(coordenadas*)malloc(sizeof(coordenadas));
			(*movimientos).posx=(*buffer).posx;
			(*movimientos).posy=(*buffer).posy-2;
			list_add(recorridoEnemigos,movimientos);
			movimientos=(coordenadas*)malloc(sizeof(coordenadas));
			(*movimientos).posx=(*buffer).posx;
			(*movimientos).posy=(*buffer).posy-3;
			list_add(recorridoEnemigos,movimientos);
			movimientos=(coordenadas*)malloc(sizeof(coordenadas));
			(*movimientos).posx=(*buffer).posx-1;
			(*movimientos).posy=(*buffer).posy-3;
			list_add(recorridoEnemigos,movimientos);
		}
		else{
			if (*ctrl<9) {
				*ctrl=*ctrl+1;
				movArribaDer(i,buffer,coorCajas,cantCajas,rows,cols,ctrl,recorridoEnemigos);
			}
		};
	}
movIzqArriba(int i,coordenadas* buffer, int coorCajas[],int cantCajas,int rows,int cols,int* ctrl,t_list* recorridoEnemigos){

	if(((*buffer).posx)-1>=0 && (((*buffer).posy)-3>=0)){ //comprueba que el movimiento que quiere hacer este dentro de los margenes del nivel
			*recorridoEnemigos=*list_create();
			coordenadas* movimientos;
			movimientos=(coordenadas*)malloc(sizeof(coordenadas));
			(*movimientos).posx=(*buffer).posx;
			(*movimientos).posy=(*buffer).posy-1;
			list_add(recorridoEnemigos,movimientos);
			movimientos=(coordenadas*)malloc(sizeof(coordenadas));
			(*movimientos).posx=(*buffer).posx;
			(*movimientos).posy=(*buffer).posy-2;
			list_add(recorridoEnemigos,movimientos);
			movimientos=(coordenadas*)malloc(sizeof(coordenadas));
			(*movimientos).posx=(*buffer).posx;
			(*movimientos).posy=(*buffer).posy-3;
			list_add(recorridoEnemigos,movimientos);
			movimientos=(coordenadas*)malloc(sizeof(coordenadas));
			(*movimientos).posx=(*buffer).posx-1;
			(*movimientos).posy=(*buffer).posy-3;
			list_add(recorridoEnemigos,movimientos);
		}
		else{
			if (*ctrl<9) {
				*ctrl=*ctrl+1;
				movArribaDer(i,buffer,coorCajas,cantCajas,rows,cols,ctrl,recorridoEnemigos);
			}
		};
}




elegirMovimiento(int i,coordenadas* buffer, int coorCajas[],int cantCajas,int rows,int cols,int cantEne,t_list *recorridoEnemigos){
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
	int ctrl;//este int sirve para que el programa se de cuenta si no se puede mover el enemigo
	switch(mov){
	case 1:
		//puts("paso 1");
		movArribaIzq(i,buffer,coorCajas,cantCajas,rows,cols,&ctrl,recorridoEnemigos);
		break;
	case 2:
		//puts("paso 2");
		movArribaDer(i,buffer,coorCajas,cantCajas,rows,cols,&ctrl,recorridoEnemigos);
		break;
	case 3:
		//puts("paso 3");
		movDerArriba(i,buffer,coorCajas,cantCajas,rows,cols,&ctrl,recorridoEnemigos);
		break;
	case 4:
		//puts("paso 4");
		movDerAbajo(i,buffer,coorCajas,cantCajas,rows,cols,&ctrl,recorridoEnemigos);
		break;
	case 5:
		//puts("paso 5");
		movAbajoDer(i,buffer,coorCajas,cantCajas,rows,cols,&ctrl,recorridoEnemigos);
		break;
	case 6:
		//puts("paso 6");
		movAbajoIzq(i,buffer,coorCajas,cantCajas,rows,cols,&ctrl,recorridoEnemigos);
		break;
	case 7:
		//puts("paso 7");
		movIzqAbajo(i,buffer,coorCajas,cantCajas,rows,cols,&ctrl,recorridoEnemigos);
		break;
	case 8:
		//puts("paso 8");
		movIzqArriba(i,buffer,coorCajas,cantCajas,rows,cols,&ctrl,recorridoEnemigos);
		break;

	}
}

moverEnemigos(t_list* listaEnemigos,t_list listaCajas,t_list listaJugadoresActivos,t_list* recorridoEnemigos[],int rows, int cols){
	int i=0;
	coordenadas* buffer;
	Caja* bufferCaja;
 recorridoEnemigos[0]=list_create();
 recorridoEnemigos[1]=list_create();
 recorridoEnemigos[2]=list_create();
 //printf("LA cant de elem de la lista %d es %d\n",0,list_size(recorridoEnemigos[0]));
 int cantCajas;
	cantCajas=list_size(&listaCajas);
	int coorCajas[cantCajas];
	int cantEne;
	cantEne=list_size(listaEnemigos);
	//t_list recorridoEnemigos[cantEne];
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
	coordenadas* proximoMov;


	if (list_size(&listaJugadoresActivos)==0){
	puts("entra al IF");

	for(i=0;i<cantEne;i++){

		if(list_size(recorridoEnemigos[i])==0){ //pregunta si no tiene movimientos para hacer el enemigo
			puts("segundo IF");
			buffer=list_get(listaEnemigos,i);
			elegirMovimiento(i,buffer,coorCajas,cantCajas,rows,cols,cantEne,recorridoEnemigos[i]);
			printf("El tamaño de la lista %d cuando sale es %d\n",i,list_size(recorridoEnemigos[i]));
		}
		else{ //aca es cuando tiene q moverse
			puts("YO TMB");

			//buffer=list_get(listaEnemigos,i);
			//proximoMov=list_remove(recorridoEnemigos[i],0);
			//(*buffer).posx=(*proximoMov).posx;
			//(*buffer).posy=(*proximoMov).posy;
			//list_replace(listaEnemigos,i,buffer);

			//actualizarNivel(listaCajas,*listaEnemigos,listaJugadoresActivos,"nivel245");
		}

		}


	}else{
		puts("no entra al if");//aca es cuando hay personajes
	}

}




