#ifndef INTODUCCION_H_
#define INTODUCCION_H_

#include <curses.h> //Incluiremos una librería a nuestro sistemas
#include <stdio.h>
#include <stdlib.h>
#include <nivel.h>
#include <unistd.h>
//#include <Plataforma.h>

typedef struct par{
	int x;
	int y;
}t_par;

typedef struct logo{
	t_par*sup_izq;
}t_logo;

void intro(void);
void salir(void);


#endif /* INTODUCCION_H_ */
