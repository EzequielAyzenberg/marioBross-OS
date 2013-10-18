/*
 * nivel.c
 *
 *  Created on: 12/10/2013
 *      Author: utnso
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <curses.h>
#include "cargador.h"


main(){
	nivelConfig config;
	cargarconfig("/home/utnso/GitMario/tp-2013-2c-the-grid/Nivel/nivel1.cfg",config);
	return 0;
}

