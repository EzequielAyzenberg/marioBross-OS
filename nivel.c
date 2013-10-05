/*
 ============================================================================
 Name        : nivel.c
 Author      : THR
 Version     : 0.1
 Description : nivel
 ============================================================================
 */
#define PERSONAJE_ITEM_TYPE 0
#define RECURSO_ITEM_TYPE 1

#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <curses.h>
#include <unistd.h>


static WINDOW * secwin;
static WINDOW * mainwin;
static int rows, cols;
static int inicializado = 0;

struct item {
	char id;
	int posx;
	int posy;
	char item_type; // PERSONAJE o CAJA_DE_RECURSOS
	int quantity;
	struct item *next;
};

typedef struct item ITEM_NIVEL;

/* PROTOTIPOS FUNCIONES */
int nivel_gui_inicializar(void);
int nivel_gui_get_area_nivel(int * rows, int * cols);
void nivel_gui_get_term_size(int * rows, int * cols);
int nivel_gui_int_validar_inicializado(void);
void nivel_gui_print_perror(const char* message);
void CrearPersonaje(ITEM_NIVEL** ListaItems, char id, int x , int y);
void CrearCaja(ITEM_NIVEL** ListaItems, char id, int x , int y, int cant);
void MoverPersonaje(ITEM_NIVEL* ListaItems, char id, int x, int y);
/* END PROTOTIPOS FUNCIONES */

int main(void) {
/*
 Compilar por cónsola de la siguiente manera:
 	 gcc nivel.c -lcurses
 */

	/*Declaración de variables*/
	ITEM_NIVEL* ListaItems = NULL;
	int rows, cols;

	/*Hardcodeo las coordenadas y el simbolo que representa
	 * al personaje para pruebas.
	 * Esto debería levantarlo del archivo de configuración
	 * del personaje*/
	int coordInitX = 1, coordInitY = 1;
	int coordX = coordInitX, coordY = coordInitY;
	char player = '@';
	/*También hardcodeo las coordenadas y el simbolo que
	 * representa a la caja de Items para pruebas.
	 * Esto también debería levantarlo del archivo de configuración
	 * */

	int coordCajaX = 20, coordCajaY = 40, cantidadCaja = 5;
	char caja = 'F';


	/*Inicializa "modo gráfico" y obtiene límites de la pantalla*/
	nivel_gui_inicializar();
	nivel_gui_get_area_nivel(&rows, &cols);

	/*Crea los personajes y las cajas*/
	/*TO DO: levantar de un archivo de configuración*/
	/*TO DO: armar una lista de Items, y ciclar hasta crearlos todos*/
	CrearPersonaje(&ListaItems, player, coordInitX, coordInitY);
	CrearCaja(&ListaItems, caja, coordCajaX, coordCajaY, cantidadCaja);

	/*Bindea personajes e items*/
	nivel_gui_dibujar(ListaItems);

	while ( 1 ) {
	/*TO DO: Agregar una señal para que agregue vidas
	 * al personaje */

	/*Lógica del calculo del movimiento
	 * Llevarla a una función y mejorarla, porque los
	 * ifs anidados no son lo más "prolijo" del mundo*/
	if (coordX < coordCajaX) {
		/*Si el personaje está a la izquierda de la caja
		 * lo muevo 1 posición a la derecha */
		coordX++;
	} else if (coordX > coordCajaX)
	{
		/*Si el personaje está a la derecha de la caja
		 * lo muevo 1 posición a la izquierda */
		coordX--;
	}
	else if (coordX == coordCajaX && coordY < coordCajaY)
	{
		/*Si el personaje está a la arriba de la caja
		 * lo muevo 1 posición hacia abajo */
		coordY++;
	} else if (coordX == coordCajaX && coordY > coordCajaY)
	{
		/*Si el personaje está a la abajo de la caja
		 * lo muevo 1 posición hacia arriba */
		coordY--;
	} else if (coordX == coordCajaX && coordY == coordCajaY) {
		/*Si el personaje está SOBRE la caja
		 * salgo del proceso
		 * TO DO: Buscar el próximo item de una lista de
		 * 		  requerimientos*/
		nivel_gui_terminar();
		exit(0);
	}

/*Pausa de un segundo para ver el movimiento*/
/*Considerar usar otra función, no sé que tan compatible es sleep*/
		sleep(1);

		MoverPersonaje(ListaItems, player, coordX, coordY);
		nivel_gui_dibujar(ListaItems);
	}

	BorrarItem(&ListaItems, player);
	BorrarItem(&ListaItems, caja);
	nivel_gui_terminar();

	return EXIT_SUCCESS;
}
/************************/
/* FUNCIONES AUXILIARES */
/************************/
/*FUNCIONES RELACIONADAS CON LOS ITEMS*/
/*TO DO: Desacoplar*/
int nivel_gui_inicializar(void) {

	if (nivel_gui_int_validar_inicializado()){
			nivel_gui_print_perror("nivel_gui_inicializar: Library ya inicializada!");
			return EXIT_FAILURE;
		}
	mainwin = initscr();
	keypad(stdscr, TRUE);
	noecho();
	start_color();
	init_pair(1,COLOR_GREEN, COLOR_BLACK);
	init_pair(2,COLOR_WHITE, COLOR_BLACK);
	init_pair(3,COLOR_BLACK, COLOR_YELLOW);
	box(stdscr, 0, 0);
	refresh();

	nivel_gui_get_term_size(&rows, &cols);
	secwin = newwin(rows - 2, cols, 0, 0);
	box(secwin, 0, 0);
	wrefresh(secwin);

	inicializado = 1;

	return EXIT_SUCCESS;
}

int nivel_gui_get_area_nivel(int * rows, int * cols) {

	if (!nivel_gui_int_validar_inicializado()){
			nivel_gui_print_perror("nivel_gui_get_area_nivel: Library no inicializada!");
			return EXIT_FAILURE;
			}

	if (rows == NULL || cols == NULL){
		nivel_gui_print_perror("nivel_gui_get_area_nivel: Ninguno de los argumentos puede ser NULL");
		return EXIT_FAILURE;
	}


	nivel_gui_get_term_size(rows, cols);
	*rows = *rows - 4;
	*cols = *cols - 2;

	return EXIT_SUCCESS;
}

int nivel_gui_terminar(void) {

		if (!nivel_gui_int_validar_inicializado()){
			nivel_gui_print_perror("nivel_gui_terminar: Library no inicializada!");
			return EXIT_FAILURE;
		}

        delwin(mainwin);
        delwin(secwin);
        endwin();
        refresh();

        return EXIT_SUCCESS;

}

void nivel_gui_get_term_size(int * rows, int * cols) {

    struct winsize ws;

    if ( ioctl(0, TIOCGWINSZ, &ws) < 0 ) {
        perror("couldn't get window size");
    }

    *rows = ws.ws_row;
    *cols = ws.ws_col;
}

int nivel_gui_int_validar_inicializado(void){
	return inicializado;
}

void nivel_gui_print_perror(const char* message){
	fprintf(stderr, "%s\n", message);
}

int nivel_gui_dibujar(ITEM_NIVEL* items) {
	if (!nivel_gui_int_validar_inicializado()){
		nivel_gui_print_perror("nivel_gui_dibujar: Library no inicializada!");
		return EXIT_FAILURE;
	}

	if (items == NULL){
		nivel_gui_print_perror("nivel_gui_dibujar: La lista de items no puede ser NULL");
		return EXIT_FAILURE;
	}

	ITEM_NIVEL *temp = items;
	int i = 0;

	werase(secwin);
	box(secwin, 0, 0);
	wbkgd(secwin, COLOR_PAIR(1));

	move(rows - 2, 2);
	printw("Recursos: ");

	while (temp != NULL) {
		wmove (secwin, temp->posx, temp->posy);
		if (temp->item_type) {
			waddch(secwin, temp->id | COLOR_PAIR(3));
		} else {
			waddch(secwin, temp->id | COLOR_PAIR(2));
		}
		if (temp->item_type) {
			move(rows - 2, 7 * i + 3 + 9);
			printw("%c: %d - ", temp->id, temp->quantity);
			i++;
		}
		temp = temp->next;

	}
	wrefresh(secwin);
	wrefresh(mainwin);

	return EXIT_SUCCESS;
}

void CrearItem(ITEM_NIVEL** ListaItems, char id, int x , int y, char tipo, int cant_rec) {
        ITEM_NIVEL * temp;
        temp = malloc(sizeof(ITEM_NIVEL));

        temp->id = id;
        temp->posx=x;
        temp->posy=y;
        temp->item_type = tipo;
        temp->quantity = cant_rec;
        temp->next = *ListaItems;
        *ListaItems = temp;
}


/*FUNCIONES RELACIONADAS CON LOS ITEMS*/
/*TO DO: Desacoplar*/
void CrearPersonaje(ITEM_NIVEL** ListaItems, char id, int x , int y) {
        CrearItem(ListaItems, id, x, y, PERSONAJE_ITEM_TYPE, 0);
}

void CrearCaja(ITEM_NIVEL** ListaItems, char id, int x , int y, int cant) {
        CrearItem(ListaItems, id, x, y, RECURSO_ITEM_TYPE, cant);
}

void MoverPersonaje(ITEM_NIVEL* ListaItems, char id, int x, int y) {

        ITEM_NIVEL * temp;
        temp = ListaItems;

        while ((temp != NULL) && (temp->id != id)) {
                temp = temp->next;
        }
        if ((temp != NULL) && (temp->id == id)) {
                temp->posx = x;
                temp->posy = y;
        }

}
