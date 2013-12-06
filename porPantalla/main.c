/*
 * main.c
 *
 *  Created on: 06/12/2013
 *      Author: utnso
 */

#include<ncurses.h>
#include<curses.h>
#include<unistd.h>

int main(){
	initscr();
	printw("HolaMundo! Para continuar toque teclas");
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
	printw("-------------------\nUsuario: %s\n",cadena);
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
	printw("Que bonitou :3 :3\n");
	refresh();
	getch();

	for(i=0;i<150;i++){
		refresh();
		printw("WEON  ");
		usleep(10 * 1000);

	}

	refresh();
	getch();

	erase();

	printw("Probando colores...");
	refresh();
	sleep(2);
	start_color(); //Esta función inicia los colores
	//Define pares de colores que serán definidos en el programa
	init_pair(1,COLOR_WHITE,COLOR_BLUE); //Texto | Fondo
	init_pair(2,COLOR_BLUE,COLOR_WHITE);
	init_pair(3,COLOR_WHITE,COLOR_BLACK);
	bkgd(COLOR_PAIR(1)); //Aqui define el color de fondo del programa
	attron(COLOR_PAIR(3));
	move(2,1); //Aqui mueve el cursor a linea 2 columna 1.
	printw("Olá mundo!!!"); //Imprimimos el texto en la posición especificada en la linea anterior.
	attroff(COLOR_PAIR(3)); /*Esta alterando el par de colores por omisión*/
	attron(COLOR_PAIR(2));
	move(3,1);
	printw("Cualquier tecla para salir"); /*Imprime el texto en la posición
	especificada en la línea anterior */
	attroff(COLOR_PAIR(2));
	refresh();
	//Actualiza la ventana
	getch();

	erase();

	bkgd(COLOR_PAIR(3));
	printw("Probando epilepsia....");
	refresh();
	sleep(1);
	printw("3...");
	refresh();
	sleep(1);
	printw("2...");
	refresh();
	sleep(1);
	printw("1...\n");
	refresh();

	for(i=0;i<150;i++){
		refresh();
		bkgd(COLOR_PAIR(2));
		usleep(10 * 1000);
		refresh();
		bkgd(COLOR_PAIR(3));
		usleep(10 * 1000);
	}

	printw("STOP!!!!");
	refresh();
	sleep(2);
	printw("    ...Presione alguna tecla para finalizar");
	getch();

	//sleep(3);//se detiene tres segundo
	endwin();
	return 0;
}
