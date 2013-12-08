
#include "Introduccion.h"

#define TOPE_HORIZONTAL "_"
#define TOPE_VERTICAL "|"
#define RETARDO_MS 50

void inicializarLogo(t_par*);
int dist_Logo_X(t_par);
int dist_Logo_Y(t_par);
void graficar_logo(t_par*,int);
void borrar_logo(t_par*);
void elegirMovimiento(t_par*);
int puedeMover(t_par*);
int distancia(t_par,t_par);
void moverAPunto(t_par*,t_par);
t_par posicionObjetivo(void);
void pintura(t_par*);
void efecto(t_par*);
void matrix(t_par*);
void salir_Intro(void);


int esHora=0;
t_window ppal;
char*logoSTR[6];


void introduction(void){
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
		init_pair(1,COLOR_BLACK,COLOR_WHITE);
		init_pair(2,COLOR_RED,COLOR_WHITE);
		init_pair(3,COLOR_GREEN,COLOR_WHITE);
		init_pair(4,COLOR_YELLOW,COLOR_WHITE);
		init_pair(5,COLOR_BLUE,COLOR_WHITE);
		init_pair(6,COLOR_MAGENTA,COLOR_WHITE);
		init_pair(7,COLOR_CYAN,COLOR_WHITE);
		init_pair(8,COLOR_WHITE,COLOR_BLACK);
		init_pair(9,COLOR_GREEN,COLOR_BLACK);
		init_pair(10,COLOR_RED,COLOR_BLACK);
		init_pair(11,COLOR_BLUE,COLOR_BLACK);
		bkgd(COLOR_PAIR(2));
		box(ppal.win, 0, 0);
		refresh();
		sleep(1);
	}
	int cont,color=1;
	t_par*logo;
	logo=(t_par*)malloc(sizeof(t_par));
	inicializarLogo(logo);
	sleep(3);
	wrefresh(ppal.win);
	borrar_logo(logo);
	wmove(ppal.win,(logo->y)+6,logo->x); wprintw(ppal.win,"                                           ");
	wrefresh(ppal.win);
	for(cont=0;cont<400;cont++){
		wrefresh(ppal.win);
		borrar_logo(logo);
		wrefresh(ppal.win);
		elegirMovimiento(logo);
		graficar_logo(logo,color);
		//wmove(ppal.win,2,2); wprintw(ppal.win,"Contador: %d",cont);
		//box(ppal.win, 0, 0);
		if(cont==100)color=3;
		if(cont==200)color=4;
		if(cont==300)color=5;
		//box(ppal.win, 0, 0);
		wrefresh(ppal.win);
		usleep(RETARDO_MS*1000);
	}
	moverAPunto(logo,posicionObjetivo());
	pintura(logo);
	sleep(1);
	efecto(logo);
	sleep(5);
	matrix(logo);
	sleep(2);
	salir_Intro();
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
			usleep(RETARDO_MS*750);
		}
		usleep(50000);
	}
	*logo=posicionObjetivo();
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
void borrar_logo(t_par*logo){
	wmove(ppal.win,(logo->y)+0,logo->x); wprintw(ppal.win,"                                           ");
	wmove(ppal.win,(logo->y)+1,logo->x); wprintw(ppal.win,"                                           ");
	wmove(ppal.win,(logo->y)+2,logo->x); wprintw(ppal.win,"                                           ");
	wmove(ppal.win,(logo->y)+3,logo->x); wprintw(ppal.win,"                                           ");
	wmove(ppal.win,(logo->y)+4,logo->x); wprintw(ppal.win,"                                           ");
	wmove(ppal.win,(logo->y)+5,logo->x); wprintw(ppal.win,"                                           ");
}
void elegirMovimiento(t_par*buffer){
	int*garbage=(int*)malloc(sizeof(int));
	static int mov=0;
	static int old=0;
	int seed;
	if(old==5)old=0;
	if (!old){
		srand(time(NULL));
		seed=rand();
		srand((int)garbage*(99999));
		seed=seed+rand();
		srand(seed);
		seed=rand();
		srand(seed);
		mov=1+rand()%4;
		free(garbage);
	}
	old++;
	if(mov==1){
		buffer->y--;	//Se mueve arrba.
		if((puedeMover(buffer)))return;
		else {
			buffer->y++;
			mov=2;
		}
	}
	if(mov==2){
		buffer->y++;	//Se mueve abajo.
		if((puedeMover(buffer)))return;
		else {
			buffer->y--;
			mov=3;
		}
	}
	if(mov==3){
		buffer->x--;	//Se mueve a izquierda.
		if((puedeMover(buffer)))return;
		else {buffer->x++;
				mov=4;}
	}
	if(mov==4){
		buffer->x++;	//Se mueve derecha.
		if((puedeMover(buffer)))return;
		else {buffer->x--;
				mov=1;}
	}
	if(mov==1){
		buffer->y--;	//Se mueve arrba.
		if((puedeMover(buffer)))return;
		else {buffer->y++;
				mov=2;}
	}
	if(mov==2){
		buffer->y++;	//Se mueve abajo.
		if((puedeMover(buffer)))return;
		else {buffer->y--;
				mov=3;}
	}
	if(mov==3){
		buffer->x--;	//Se mueve a izquierda.
		if((puedeMover(buffer)))return;
		else {buffer->x++;
				mov=4;}
	}
}
int puedeMover(t_par*buffer){
	if(buffer->x<2)return 0;
	if(dist_Logo_X(*buffer)>(ppal.lim->x)-2) return 0;
	if(buffer->y<2)return 0;
	if(dist_Logo_Y(*buffer)>(ppal.lim->y)-2) return 0;
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
	graficar_logo(logo,2);
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
		graficar_logo(logo,2);
		wrefresh(ppal.win);
		usleep(RETARDO_MS*5000);
	}
}
t_par posicionObjetivo(void){
	t_par posicion;
	posicion.x=(ppal.lim->x/2)-21;
	posicion.y=(ppal.lim->y/2)-3;
	return posicion;
}
void pintura(t_par*logo){
	int i,j;
	t_par aux;
	aux=posicionObjetivo();
	for(i=0;i<=ppal.lim->y;i++){
		wrefresh(ppal.win);
		for(j=0;j<=ppal.lim->x;j++){
			if (has_colors())attron(COLOR_PAIR(8));
			wmove(ppal.win,i,j); wprintw(ppal.win," ");
			if (has_colors())attroff(COLOR_PAIR(8));
		}
		if(i==(aux.y)+0){attron(COLOR_PAIR(8));wmove(ppal.win,i,aux.x);wprintw(ppal.win,(char*)logoSTR[0]);attroff(COLOR_PAIR(8));}
		if(i==(aux.y)+1){attron(COLOR_PAIR(8));wmove(ppal.win,i,aux.x);wprintw(ppal.win,(char*)logoSTR[1]);attroff(COLOR_PAIR(8));}
		if(i==(aux.y)+2){attron(COLOR_PAIR(8));wmove(ppal.win,i,aux.x);wprintw(ppal.win,(char*)logoSTR[2]);attroff(COLOR_PAIR(8));}
		if(i==(aux.y)+3){attron(COLOR_PAIR(8));wmove(ppal.win,i,aux.x);wprintw(ppal.win,(char*)logoSTR[3]);attroff(COLOR_PAIR(8));}
		if(i==(aux.y)+4){attron(COLOR_PAIR(8));wmove(ppal.win,i,aux.x);wprintw(ppal.win,(char*)logoSTR[4]);attroff(COLOR_PAIR(8));}
		if(i==(aux.y)+5){attron(COLOR_PAIR(8));wmove(ppal.win,i,aux.x);wprintw(ppal.win,(char*)logoSTR[5]);attroff(COLOR_PAIR(8));}
		wrefresh(ppal.win);
		usleep(4000*RETARDO_MS);
	}
	if (has_colors())attroff(COLOR_PAIR(8));
	bkgd(COLOR_PAIR(8));
}
void efecto(t_par*logo){
	int i;
	t_par aux;
	aux=posicionObjetivo();
	for(i=0;i<51;i++){
		wrefresh(ppal.win);
		graficar_logo(&aux,9);
		wrefresh(ppal.win);
		usleep(10 * 1000);
		graficar_logo(&aux,10);
		wrefresh(ppal.win);
		usleep(10 * 1000);
		graficar_logo(&aux,11);
		wrefresh(ppal.win);
		usleep(10 * 1000);
	}
	wrefresh(ppal.win);


}
int randomMatrix(int seed1,int seed2,int base){
	int*garbage1=(int*)malloc(sizeof(int));
	int*garbage2=(int*)malloc(sizeof(int));
	int*garbage3=(int*)malloc(sizeof(int));
	int*garbage4=(int*)malloc(sizeof(int));
	int seed;
	srand(time(NULL));
	seed=rand();
	srand(seed1*312);
	seed=rand();
	srand((int)garbage1/(99999));
	seed=seed+rand();
	srand((int)garbage2*(2222));
	seed=rand();
	srand(seed);
	seed=rand();
	srand(abs(seed/time(NULL)));
	seed=rand();
	srand(seed*seed1);
	seed=rand();
	srand(seed-seed2);
	seed=rand();
	srand(abs(seed2*99213-seed));
	seed=rand();
	srand(seed2*133*seed);
	seed=rand();
	srand(seed1*seed);
	seed=rand();
	srand(abs(seed-time(NULL)));
	seed=rand();
	srand(seed);
	seed=rand();
	srand(seed*(int)garbage4*(int)garbage3);
	seed=rand();
	free(garbage1);
	free(garbage2);
	free(garbage3);
	free(garbage4);
	return rand()%base;
}
void matrix(t_par*logo){
	int i,x,y,ran1,ran2,ran3;
	if (has_colors())attron(COLOR_PAIR(9));
	for(i=0;i<1500;i++){
		wrefresh(ppal.win);
		y=(int)randomMatrix(x,y,ppal.lim->y);
		x=(int)randomMatrix(x,y,ppal.lim->x);
		ran1=32+randomMatrix(x,y,93);
		ran2=32+randomMatrix(x,y,53);
		ran3=42+randomMatrix(x,y,53);
		wmove(ppal.win,y,x); wprintw(ppal.win,"%c%c %c",(char)ran1,(char)ran2,(char)ran3);
		wrefresh(ppal.win);
		usleep(10 * 100);
	}
	if (has_colors())attroff(COLOR_PAIR(9));
}




void salir_Intro(void){
wrefresh(ppal.win);
bkgd(COLOR_PAIR(8));
wrefresh(ppal.win);
endwin(); /*Siempre que finalizamos un programa con una biblioteca curses, debemos ejecutar este comando.*/
//exit(0);
}









