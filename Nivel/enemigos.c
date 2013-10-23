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

	list_create(listaEnemigos);
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






