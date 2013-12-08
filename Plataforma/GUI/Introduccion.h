#ifndef INTODUCCION_H_
#define INTODUCCION_H_

#include <curses.h> //Incluiremos una librería a nuestro sistemas
#include <stdio.h>
#include <stdlib.h>
#include <nivel.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <signal.h>


bool finalizar;


void cerrarTodo(int senial);


typedef struct par{
	int x;
	int y;
}t_par;

typedef struct ventana{
	WINDOW*win;
	t_par*lim;
}t_window;

typedef struct logo{
	t_par*sup_izq;
}t_logo;


//int esHora=0;
t_window ppal;
char*logoSTR[6];


void introduction(void);
int inicializarLogo(t_par*);
int dist_Logo_X(t_par);
int dist_Logo_Y(t_par);
void graficar_logo(t_par*,int);
void borrar_logo(t_par*);
void elegirMovimiento(t_par*);
int puedeMover(t_par*);
int distancia(t_par,t_par);
int moverAPunto(t_par*,t_par);
t_par posicionObjetivo(void);
int pintura(t_par*);
int efecto(t_par*);
int matrix(t_par*);
void salir_Intro(void);


#endif /* INTODUCCION_H_ */
