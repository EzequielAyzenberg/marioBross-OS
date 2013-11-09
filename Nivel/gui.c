/*
 * gui.c
 *
 *  Created on: 21/10/2013
 *      Author: utnso
 */
//#include <nivel.h>
#include <tad_items.h>
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <commons/collections/list.h>
#include "cargador.h"
#include "enemigos.h"
#include "personajes.h"
#include <theGRID/sockets.h>


 void inicializarNivel(nivelConfig config,int* rows,int*cols){

	 ITEM_NIVEL* items = NULL;

	int cantCajas,i;
	Caja* buffer;
	//t_list* items;

	//items=list_create();

	nivel_gui_inicializar();

	nivel_gui_get_area_nivel(rows, cols);

	cantCajas=list_size(&config.listaCajas);

	for(i=0;i<cantCajas;i++){
		buffer=list_get(&config.listaCajas,i);
		CrearCaja(&items,(*buffer).id,(*buffer).posx,(*buffer).posy,(*buffer).quantity);
	}






			nivel_gui_dibujar(items,config.nombre);

			//list_destroy(items);
}

 void actualizarNivel(t_list listaCajas,t_list listaEnemigos,t_list listaJugadoresActivos,char* nombre){
	 int cantItems=0;

	int cantCajas,cantEne,cantPj,i;

	Caja* bufferCaja;
	coordenadas* bufferEnemigos;
	t_personaje* bufferPersonaje;
	ITEM_NIVEL* items = NULL;
	//t_list* items;
	//items=list_create();
	cantCajas=list_size(&listaCajas);
	cantEne=list_size(&listaEnemigos);
	cantPj=list_size(&listaJugadoresActivos);
	for(i=0;i<cantCajas;i++){
		bufferCaja=list_get(&listaCajas,i);
		CrearCaja(&items,(*bufferCaja).id,(*bufferCaja).posx,(*bufferCaja).posy,(*bufferCaja).quantity);

	}
	i=0;

	for(i=0;i<cantEne;i++){
	//	char id[1];


		//itoa(i+1,id); //itoa() esta declarada en cargador.h
		bufferEnemigos=list_get(&listaEnemigos,i);
		//printf("%s\n",id);

		CrearEnemigo(&items,i+1,(*bufferEnemigos).posx,(*bufferEnemigos).posy);

	}
	i=0;


	for(i=0;i<cantPj;i++){
		bufferPersonaje=list_get(&listaJugadoresActivos,i);
		CrearPersonaje(&items,(*bufferPersonaje).id,(*bufferPersonaje).posx,(*bufferPersonaje).posy);
	}


	nivel_gui_dibujar(items,nombre);
	ITEM_NIVEL* itemsBuffer = NULL;

	while(items->next!=NULL){
		itemsBuffer=items;
		items=items->next;
		free(itemsBuffer);

	};
	free(items);
	//list_destroy(items);
	//free(bufferCaja);
	//free(bufferEnemigos);
	//free(bufferPersonaje);
	//int as;
		//scanf("%d\n",&as);
		//flush_in();
		//scanf("%d\n",&as);
		//nivel_gui_terminar();

}

