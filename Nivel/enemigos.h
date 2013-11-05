/*
 * enemigos.h
 *
 *  Created on: 22/10/2013
 *      Author: utnso
 */

#ifndef ENEMIGOS_H_
#define ENEMIGOS_H_
typedef struct datosEnemigos{
	int posx;
	int posy;
}coordenadas;

typedef struct estructuraHiloEnemigos{
	t_list* listaEnemigos;
	t_list* listaCajas;
	t_list* listaJugadoresActivos;
	t_list* listaJugadoresMuertos;
	int rows;
	int cols;
	int sleepEnemigos;
	char* nombreNivel;
	int cantEne;
	coordenadas recorridoEnemigos[][4];
}infoEnemigosThread;

void controlEnemigos(infoEnemigosThread* info);

void movArribaIzq(int i,coordenadas* buffer,int rows,int cols,int* ctrl,coordenadas recorridoEnemigos[]);
void movArribaDer(int i,coordenadas* buffer,int rows,int cols,int* ctrl,coordenadas recorridoEnemigos[]);
void movDerArriba(int i,coordenadas* buffer,int rows,int cols,int* ctrl,coordenadas recorridoEnemigos[]);
void movDerAbajo(int i,coordenadas* buffer,int rows,int cols,int* ctrl,coordenadas recorridoEnemigos[]);
void movAbajoDer(int i,coordenadas* buffer,int rows,int cols,int* ctrl,coordenadas recorridoEnemigos[]);
void movAbajoIzq(int i,coordenadas* buffer,int rows,int cols,int* ctrl,coordenadas recorridoEnemigos[]);
void movIzqAbajo(int i,coordenadas* buffer,int rows,int cols,int* ctrl,coordenadas recorridoEnemigos[]);
void movIzqArriba(int i,coordenadas* buffer,int rows,int cols,int* ctrl,coordenadas recorridoEnemigos[]);

#endif /* ENEMIGOS_H_ */
