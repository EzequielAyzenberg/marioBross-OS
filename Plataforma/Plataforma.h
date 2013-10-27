/*
 * plataforma.h
 *
 *  Created on: 27/10/2013
 *      Author: utnso
 */

#ifndef PLATAFORMA_H_
#define PLATAFORMA_H_

/* Nodos de la lista NOVEDADES */
typedef struct tnuevo{
	int pid;
	char sym;
	struct tnuevo* sgte;
}nuevo;

/* Nodos que contienen listas de novedades */
typedef struct tnodoNivel{
	nuevo tanda;
	nuevo tandaActual;
	char nombreNivel[13];
	int cnatJugadores;
}nodoNivel;

/* Registro que necesito el orquestador
 * al ser llamado por la plataforma
 */typedef struct tinfoOrquestador{
	 int puerto;
	 t_list *listaNiveles;
 }infoOrquestador;

/* Registro que necesita el planificador
 * al ser llamado por el orquestador
 */typedef struct tinfo{
	int nid;
	char name[17];
	int currplay;
	nuevo* new;
	struct tinfo* sgte;
}info;

/* Crea una tanda vacia con el argumento
 * apuntando a la cabeza de la lista
 */void crearTanda(nuevo**);

#endif /* PLATAFORMA_H_ */
