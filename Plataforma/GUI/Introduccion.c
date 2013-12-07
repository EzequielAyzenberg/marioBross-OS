
#include "Introduccion.h"

#define TOPE_HORIZONTAL "_"
#define TOPE_VERTICAL "|"

void intro(void){
	WINDOW *win;
	int rows,cols;
	nivel_gui_get_term_size(&rows,&cols);
	printf("CANTIDAD DE COLUMNAS: %d\n",cols);
	printf("CANTIDAD DE FILA: %d\n",rows);
	sleep(5);
	win=initscr();
	//keypad(stdscr, TRUE);
	if (has_colors()) {
		start_color();
		init_pair(1,COLOR_YELLOW,COLOR_BLUE);
		init_pair(2,COLOR_RED,COLOR_WHITE);
		init_pair(3,COLOR_BLACK,COLOR_WHITE);
		init_pair(4,COLOR_MAGENTA,COLOR_CYAN);
		init_pair(5,COLOR_CYAN,COLOR_GREEN);
		bkgd(COLOR_PAIR(2));
		box(win, 0, 0);
		refresh();
		/*attron(COLOR_PAIR(1));
		attroff(COLOR_PAIR(1));*/
		sleep(5);


		/*
		      ####### #  # ####		##### #### ##### ####
		         #    #  # #   		#     #  #   #   #   #
		         # 	  #### ####  	# ### ###    #   #   #
		         #    #  # #     	#   # # #    #   #   #
		         #    #  # ####     ##### #  # ##### ####
		                          	  	  	 version-2.0-

		*/
	}
	salir();
}

t_par esquina_sup_der(t_logo theGRID){
	t_par aux;
	aux.x=(theGRID.sup_izq->x)+40;
	aux.y=theGRID.sup_izq->y;
	return aux;
}
t_par esquina_inf_izq(t_logo theGRID){
	t_par aux;
	aux.x=theGRID.sup_izq->x;
	aux.y=(theGRID.sup_izq->y)+5;
	return aux;
}
t_par esquina_inf_der(t_logo theGRID){
	t_par aux;
	aux.x=(theGRID.sup_izq->x)+40;
	aux.y=(theGRID.sup_izq->y)+5;
	return aux;
}

int graficar_logo(t_logo theGRID,t_par start){

return 1;
}



	/*int i;char c;
	initscr();
	c=95;//caracter ascci horizontal
	for(i=1;i<=120;i++){//linea horizontal superior
		move(1,i); //Aqui estamos moviendo el cursor para a linea 1 columna i.
		printw("%c",c); //Imprimimos un texto en la posición establecida. //linea horizontal inferior
		move(40,i); //Aqui estamos moviendo el cursor para a linea 40 columna i.
		printw("%c",c); //Imprimimos un texto en la posición establecida.
	}
	c=124 ; //caracter ascci vertical
	for(i=2;i<=40;i++){//linea vertical izquierda
		move(i,1);
		printw("%c",c);	//linea vertical derecha
		move(i,120);
		printw("%c",c);
	}
	refresh();
	//getch(); //si se desea hasta que se pulse una tecla
	sleep(3);//se detiene tres segundo
	salir();
	// Salir del programa
*/

	//win=initscr(); /* Crea la ventana */
	//clear(); /* Borra la pantalla entera bajo ncurses */
	//refresh(); /* Actualiza la ventana con los cambios */
	//noecho();
	//cbreak();
	//keypad(win, TRUE);


/*********************************************************************/

void salir()
{
endwin(); /*Siempre que finalizamos un programa con una biblioteca curses, debemos ejecutar este comando.*/
//exit(0);
}
