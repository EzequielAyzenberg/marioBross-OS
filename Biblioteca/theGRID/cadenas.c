/*
 * cadenas.c
 *
 *  Created on: 14/10/2013
 *      Author: alumno
 */
#include "cadenas.h"

int leecad(char *cad, int n) {
    int i, c;

    c=getchar();
    if (c == EOF) {
        cad[i] = '\0';
        return 0;
    }

    if (c == '\n')
        i = 0;
    else {
        cad[0] = c;
        i = 1;
    }

    for (; i < n-1 && (c=getchar())!=EOF && c!='\n'; i++)
       cad[i] = c;

    cad[i] = '\0';

    if (c != '\n' && c != EOF)
        while ((c = getchar()) != '\n' && c != EOF);

    return 1;
}
