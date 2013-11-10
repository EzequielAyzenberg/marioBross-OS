/*
 * cadenas.c
 *
 *  Created on: 14/10/2013
 *      Author: alumno
 */
#include "cadenas.h"

int leecad(char *cad, int n) {
    int i=0, c;
    c=getchar();
    if (c == EOF) {
        cad[i] = '\0';
        return 0;
    }
    if (c == '\n') i = 0;
    else{
        cad[0] = c;
        i = 1;	}
    for(; i < n-1 && (c=getchar())!=EOF && c!='\n'; i++)
       cad[i] = c;
    cad[i] = '\0';
    if(c != '\n' && c != EOF) while ((c = getchar()) != '\n' && c != EOF);
    return 1;
}

char* concat(char *s1, char *s2){
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the zero-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

char* ctos(char caracter){
	char * s = (char*)malloc(sizeof(char[2]));
	s[0] = caracter;
	s[1] = '\0';
	return s;
}
