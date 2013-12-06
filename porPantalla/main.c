/*
 * main.c
 *
 *  Created on: 06/12/2013
 *      Author: utnso
 */

#include<ncurses.h>
#include<unistd.h>

int main(){
	initscr();
	printw("HolaMundo!");
	refresh();
	getch();

	erase();

	char cadena[128], pass[12];
	printw("Usuario: ");
	scanw("%s",cadena);
	printw("Contraseña: ");
	noecho();
	scanw("%s",pass);
	echo();
	printw("Usuario: %s\n",cadena);
	printw("Contraseña: %s\n\n Te re cabió",pass);
	refresh();
	getch();

	erase();

	/*dibujar_cuadro.c*/
	int i;char c;
	initscr();
	c=95;//caracter ascci horizontal
	for(i=1;i<=20;i++){
		refresh();
		//linea horizontal superior
		move(1,i); //Aqui estamos moviendo el cursor para a linea 1 columna i.
		printw("%c",c); //Imprimimos un texto en la posición establecida.
		//linea horizontal inferior
		move(20,i); //Aqui estamos moviendo el cursor para a linea 40 columna i.
		printw("%c",c); //Imprimimos un texto en la posición establecida.
		usleep(100 * 1000);
	}
	c=124 ; //caracter ascci vertical
	for(i=2;i<=20;i++){
		refresh();
		//linea vertical izquierda
		move(i,1);
		printw("%c",c);
		//linea vertical derecha
		move(i,20);
		printw("%c",c);
		usleep(100 * 1000);
	}
	refresh();
	getch();

	//sleep(3);//se detiene tres segundo
	endwin();
	return 0;
}
