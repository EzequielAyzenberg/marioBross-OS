#ifndef INTODUCCION_H_
#define INTODUCCION_H_

#include <curses.h> //Incluiremos una librería a nuestro sistemas
#include <stdio.h>
#include <stdlib.h>
#include <nivel.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

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


void introduction(void);
void salir(void);


#endif /* INTODUCCION_H_ */
