/*
 * prototipo.c
 *
 *  Created on: 07/12/2013
 *      Author: utnso
 */

#define STATUS_ROW 5
#include "Introduccion.h"

int rows,cols, miniRow;
div_t resultado;
void nuevoPanel(WINDOW* win, int posY);
void nuevoStatus(WINDOW* statusWin, WINDOW* koopaWin);

int main(){
	nivel_gui_get_term_size(&rows,&cols);

	if(cols<80 || rows<24){
		printf("\n--La terminal debe ser minimo de 80x24\n\n");
		return 0;
	}

	initscr();

	start_color();
	init_pair(52,COLOR_BLUE,COLOR_BLACK);
	init_pair(53,COLOR_WHITE,COLOR_BLACK);

	resultado = div(rows-STATUS_ROW,3);
	miniRow = resultado.quot;

	printw("CANTIDAD DE COLUMNAS: %d\n",cols);
	printw("CANTIDAD DE FILA: %d\n",rows);

	refresh();
	getch();

	clear();

	WINDOW *win1,*win2,*win3,*statusWin,*koopaWin;
	nuevoPanel(win1,0);
	nuevoPanel(win2,miniRow);
	nuevoPanel(win3,miniRow*2);
	nuevoStatus(statusWin,koopaWin);

	refresh();
	getch();

	endwin();
	return 0;
}

void nuevoPanel(WINDOW* win, int posY){
	refresh();
	win=newwin(miniRow,cols,posY,0);
	wbkgd(win,COLOR_PAIR(53));
	box(win, 0, 0);
	mvwprintw(win,0,1,"Planificador");
	wrefresh(win);
	return;
};

void nuevoStatus(WINDOW* statusWin, WINDOW* koopaWin){
	refresh();
	div_t mitad = div(cols,2);
	int ancho = mitad.quot;
	int alto = STATUS_ROW + resultado.rem;
	int posY = miniRow*3;
	int posX = 0;
	statusWin = newwin(alto,ancho,posY,posX);
	wbkgd(statusWin,COLOR_PAIR(53));
	box(statusWin, 0, 0);
	mvwprintw(statusWin,0,1,"StatusBar");
	wrefresh(statusWin);

	ancho = mitad.quot + mitad.rem;
	posX = mitad.quot;
	koopaWin = newwin(alto,ancho,posY,posX);
	wbkgd(koopaWin,COLOR_PAIR(52));
	box(koopaWin, 0, 0);
	mvwprintw(koopaWin,0,1,"KoopaBar");
	wrefresh(koopaWin);
	return;
};
