/*
 * gui.c
 *
 *  Created on: 21/10/2013
 *      Author: utnso
 */
#include <tad_items.h>
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <commons/collections/list.h>

dibujarNivel(){
	int rows,cols;
	t_list* items;
	items=list_create();
	nivel_gui_inicializar();
	nivel_gui_get_area_nivel(&rows, &cols);

	CrearPersonaje(items, '@', 1, 1);
	CrearPersonaje(items, '#', 10, 10);
	CrearEnemigo(items, '1', 20, 20);
	CrearEnemigo(items, '2', 10, 10);

	CrearCaja(items, 'M', 8, 15, 3);
	CrearCaja(items, 'F', 19, 9, 2);
	CrearCaja(items, 'H', 26, 10, 5);
	nivel_gui_dibujar(items,"Mi primer nivel");
	scanf("%d",&rows);
	nivel_gui_terminar();
}
