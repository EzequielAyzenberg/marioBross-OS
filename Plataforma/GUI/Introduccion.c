
#include "Introduccion.h"

#define TOPE_HORIZONTAL "_"
#define TOPE_VERTICAL "|"
#define RETARDO_MS 50

void inicializarLogo(t_par*);
int dist_Logo_X(t_par);
int dist_Logo_Y(t_par);
void graficar_logo(t_par*,int);
void elegirMovimiento(t_par*);
int puedeMover(t_par*);
int distancia(t_par,t_par);
void moverAPunto(t_par*,t_par);
t_par posicionObjetivo(void);



int esHora=0;
t_window ppal;
char*logoSTR[6];


void intro(void){
	int j,len;
	len=strlen("###########################################")+1;
	for(j=0;j<6;j++){
		logoSTR[j]=(char*)malloc(len);
	}
	strcpy(logoSTR[0],"####### #  # ####    ##### #### ##### #### ");
	strcpy(logoSTR[1],"   #    #  # #       #     #  #   #   #   #");
	strcpy(logoSTR[2],"   #    #### ####    # ### ###    #   #   #");
	strcpy(logoSTR[3],"   #    #  # #       #   # # #    #   #   #");
	strcpy(logoSTR[4],"   #    #  # ####    ##### #  # ##### #### ");
	strcpy(logoSTR[5],"                              version-2.0- ");
	int rows,cols;
	t_par auxPar;
	nivel_gui_get_term_size(&rows,&cols);
	auxPar.x=cols;
	auxPar.y=rows;
	ppal.lim=&auxPar;
	printf("CANTIDAD DE COLUMNAS: %d\n",cols);
	printf("CANTIDAD DE FILA: %d\n",rows);
	sleep(1);
	ppal.win=initscr();
	//keypad(stdscr, TRUE);
	if (has_colors()) {
		start_color();
		init_pair(1,COLOR_YELLOW,COLOR_BLUE);
		init_pair(2,COLOR_RED,COLOR_WHITE);
		init_pair(3,COLOR_BLACK,COLOR_WHITE);
		init_pair(4,COLOR_MAGENTA,COLOR_CYAN);
		init_pair(5,COLOR_CYAN,COLOR_GREEN);
		bkgd(COLOR_PAIR(1));
		box(ppal.win, 0, 0);
		refresh();
		sleep(1);
	}
	int cont,color=1;
	t_par *buffer,*logo;
	logo=(t_par*)malloc(sizeof(t_par));
	buffer=(t_par*)malloc(sizeof(t_par));
	inicializarLogo(logo);
	sleep(5);
	buffer=logo;
	for(cont=0;cont<1000;cont++){
		werase(ppal.win);
		do{
			elegirMovimiento(buffer);
		}while(!puedeMover(buffer));
		graficar_logo(logo,color);
		if(cont==250)color=2;
		if(cont==500)color=3;
		if(cont==750)color=4;
		box(ppal.win, 0, 0);
		wrefresh(ppal.win);
		usleep(RETARDO_MS*1000);
	}
	moverAPunto(logo,posicionObjetivo());





	salir();
}
void inicializarLogo(t_par*logo){
	t_par aux;
	int i;
	for(i=0;i<6;i++){
		aux.x=(ppal.lim->x/2)-21;
		aux.y=ppal.lim->y-2;
		while(distancia(aux,posicionObjetivo())-i){
			//werase(ppal.win);
			wrefresh(ppal.win);
			box(ppal.win, 0, 0);
			wmove(ppal.win,aux.y,aux.x); wprintw(ppal.win,logoSTR[i]);
			if(aux.y!=ppal.lim->y-2)wmove(ppal.win,aux.y+1,aux.x); wprintw(ppal.win,"                                           ");
			wrefresh(ppal.win);
			aux.y--;
			usleep(RETARDO_MS*1000);
		}
		sleep(1);
	}
	wrefresh(ppal.win);
	//wmove(ppal.win,5,30); wprintw(ppal.win,"FAIL");
	wrefresh(ppal.win);
}
int dist_Logo_X(t_par pos){
	return pos.x+42;
}
int dist_Logo_Y(t_par pos){
	return pos.y+5;
}
void graficar_logo(t_par*logo,int color){
	if (has_colors())attron(COLOR_PAIR(color));
	wmove(ppal.win,(logo->y)+0,logo->x); wprintw(ppal.win,(char*)logoSTR[0]);
	wmove(ppal.win,(logo->y)+1,logo->x); wprintw(ppal.win,(char*)logoSTR[1]);
	wmove(ppal.win,(logo->y)+2,logo->x); wprintw(ppal.win,(char*)logoSTR[2]);
	wmove(ppal.win,(logo->y)+3,logo->x); wprintw(ppal.win,(char*)logoSTR[3]);
	wmove(ppal.win,(logo->y)+4,logo->x); wprintw(ppal.win,(char*)logoSTR[4]);
	wmove(ppal.win,(logo->y)+5,logo->x); wprintw(ppal.win,(char*)logoSTR[5]);
	if (has_colors())attroff(COLOR_PAIR(color));
}
void elegirMovimiento(t_par*buffer){
	int*i=(int*)malloc(sizeof(int));
	int mov;
	int seed;
	srand(time(NULL));
	seed=rand();
	srand((int)i*(99999));
	seed=seed+rand();
	srand(seed);
	seed=rand();
	srand(seed);
	mov=1+rand()%4;
	free(i);
	switch(mov){
	case 1:buffer->y--;	//Se mueve arrba.
		break;
	case 2:buffer->y++; //Se mueve abajo.
		break;
	case 3:buffer->x--; //Se mueve a izquierda.
		break;
	case 4:buffer->x++; //Se mueve a derecha.
		break;
	}
}
int puedeMover(t_par*buffer){
	if(buffer->x<1)return 0;
	if(dist_Logo_X(*buffer)>=ppal.lim->x) return 0;
	if(buffer->y<1)return 0;
	if(dist_Logo_Y(*buffer)>=ppal.lim->y) return 0;
	return 1;
}
int distancia(t_par origen,t_par destino){
	int dist;
	dist=abs(origen.x-destino.x);
	dist+=abs(origen.y-destino.y);
	return dist;
}
void moverAPunto(t_par*logo,t_par objetivo){
	wrefresh(ppal.win);
	wmove(ppal.win,5,30); wprintw(ppal.win,"FAIL");
	wrefresh(ppal.win);
	int moverEnX=1;
	while(distancia(*logo,objetivo)){
		werase(ppal.win);
		if(moverEnX){
			if(logo->x<objetivo.x)logo->x++;
			else logo->x--;
			moverEnX=0;
		}else{
			if(logo->y<objetivo.y)logo->y++;
			else logo->y--;
			moverEnX=1;
		}
		if(logo->x==objetivo.x) moverEnX=0;
		if(logo->y==objetivo.y) moverEnX=1;
		wrefresh(ppal.win);
		box(ppal.win, 0, 0);
		graficar_logo(logo,2);
		wrefresh(ppal.win);
		usleep(RETARDO_MS*1000);
	}
}
t_par posicionObjetivo(void){
	t_par posicion;
	posicion.x=(ppal.lim->x/2)-21;
	posicion.y=(ppal.lim->y/2)-3;
	return posicion;
}







void salir(){
endwin(); /*Siempre que finalizamos un programa con una biblioteca curses, debemos ejecutar este comando.*/
//exit(0);
}









