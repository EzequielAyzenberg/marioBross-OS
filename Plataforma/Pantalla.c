#define STATUS_ROW 7
#define PLANI_ROW 22
#include "Pantalla.h"

int rows,cols;
WINDOW* nuevoPanel(int posY);
void _pantallaNivel(nodoNivel*);
bool _sePuedeDibujar(nodoNivel*);
void interrupcionPlani(nodoNivel*);
void dibujarBarra(int);
void dibujarScroll();
void * scroller(void*);
void cleanPlanificador();

//WINDOW* ppal;
extern bool mpantalla, mnormal, pantallaTerminada;
extern t_list *listaNiveles;
extern pthread_mutex_t mutexInterr;
int resultado, planiRow;
WINDOW*scrollWin,*barraWin,
*statusWin=NULL,*koopaWin=NULL;
bool sePuedeBorrar;

void pantallaKoopa(char*mensaje){
	refresh();
	div_t tercera = div(cols,2);
	int ancho,alto = STATUS_ROW -2,posY = 1,posX;
	ancho = tercera.quot + tercera.rem;
	posX = tercera.quot;
	koopaWin = newwin(alto,ancho,posY,posX);
	wbkgd(koopaWin,COLOR_PAIR(58)|A_BOLD);
	box(koopaWin, 0, 0);
	mvwprintw(koopaWin,0,1,"StatusBar - Koopa");
	posX=cols/4-(strlen(mensaje)/2);
	posY=STATUS_ROW/2-1;
	mvwprintw(koopaWin,posY,posX,"                    ");
	mvwprintw(koopaWin,posY,posX,mensaje);
	wrefresh(koopaWin);
	return;
}

void settearPantallaKoopa(){
	div_t tercera = div(cols,2);
	int ancho,alto = STATUS_ROW -2,posY = 1,posX;
	ancho = tercera.quot + tercera.rem;
	posX = tercera.quot;
	koopaWin = newwin(alto,ancho,posY,posX);
	wbkgd(koopaWin,COLOR_PAIR(50));
	box(koopaWin, 0, 0);
	mvwprintw(koopaWin,0,1,"StatusBar - Koopa");
	wrefresh(koopaWin);
	wbkgd(koopaWin,COLOR_PAIR(50)|A_BOLD);
	char mensaje[16];
	strcpy(mensaje,"KOOPA INACTIVO");
	posX=cols/4-(strlen(mensaje)/2);
	posY=STATUS_ROW/2-1;
	mvwprintw(koopaWin,posY,posX,mensaje);
	wrefresh(koopaWin);
	return;
}

void pantallaStatus(char*mensaje){
	settearStatus();
	refresh();
	box(statusWin, 0, 0);
	mvwprintw(statusWin,0,1,"StatusBar - Orquestador");
	int posX=cols/4-(strlen(mensaje)/2);
	int posY=STATUS_ROW/2-1;
	mvwprintw(statusWin,posY,posX,mensaje);
	wrefresh(statusWin);
	return;
}

void settearStatus(){
	refresh();
	div_t tercera = div(cols,2);
	int ancho = tercera.quot;
	int alto = STATUS_ROW -2;
	int posY = 1;
	int posX = 0;
	statusWin = newwin(alto,ancho,posY,posX);
	wbkgd(statusWin,COLOR_PAIR(51));
	box(statusWin, 0, 0);
	mvwprintw(statusWin,0,1,"StatusBar - Orquestador");
	wrefresh(statusWin);
	return;
};

void dibujarScroll(){
	refresh();
	scrollWin=(WINDOW*)malloc(sizeof(WINDOW));
	barraWin=(WINDOW*)malloc(sizeof(WINDOW));
	scrollWin=newwin(planiRow+2,1,STATUS_ROW-1,cols -2);
	wbkgd(scrollWin,COLOR_PAIR(51)|A_BOLD);
	wrefresh(scrollWin);
	return;
}

void dibujarBarra(int indice){

	dibujarScroll();

	int alto,posY,posX;
	alto = planiRow/list_size(listaNiveles);
	posY=STATUS_ROW+indice*alto;
	posX=cols-2;
	refresh();
	//barraWin=subwin(scrollWin,alto,1,posY,0);
	barraWin=newwin(alto,1,posY,posX);
	wbkgd(barraWin,COLOR_PAIR(56));
	wrefresh(barraWin);
	refresh();
	return;
}

void * scroller(void *name){
	keypad(stdscr, TRUE);

	while(listaNiveles->elements_count<2);
	dibujarBarra(0);
	int ch, actual=0;
	nodoNivel*nodoActual;
	nodoActual = list_get(listaNiveles,actual);
	    while ((ch = getch()) != EOF){

	    	if (ch == KEY_UP && actual>0){
	    		nodoActual->dibujar = false;
	    		actual--;
	    		nodoActual = list_get(listaNiveles,actual);
	    		nodoActual->dibujar = true;
	        }
	        else if (ch == KEY_DOWN &&
	     actual<listaNiveles->elements_count-1){
	        	nodoActual->dibujar = false;
	        	actual++;
	        	nodoActual = list_get(listaNiveles,actual);
	        	nodoActual->dibujar = true;
	        }
	    	dibujarBarra(actual);
	    }
	return NULL;
}


WINDOW*wind,*swin,*salert,*bloque;

void *pantalla(void*parametro){
	sePuedeBorrar=true;
	nivel_gui_get_term_size(&rows,&cols);
	int planiRowActual = rows-STATUS_ROW-2;
	if(planiRowActual >= PLANI_ROW) planiRow = planiRowActual;
	else planiRow = PLANI_ROW;
	resultado=cols/5;
	pthread_t hiloScroll;
	if(cols<80 || rows<24){
		printf("\n--La terminal debe ser minimo de 80x24\n\n");
		mpantalla=false;
		return 0;
	}
	initscr();
	start_color();
	init_pair(50,COLOR_BLUE,COLOR_BLACK);
	init_pair(51,COLOR_WHITE,COLOR_BLACK);
	init_pair(52,COLOR_GREEN,COLOR_BLACK);
	init_pair(53,COLOR_YELLOW,COLOR_BLACK);
	init_pair(54,COLOR_RED,COLOR_BLACK);
	init_pair(55,COLOR_CYAN,COLOR_BLACK);
	init_pair(56,COLOR_BLACK,COLOR_WHITE);
	init_pair(57,COLOR_RED,COLOR_RED);
	init_pair(58,COLOR_WHITE,COLOR_RED);
	refresh();
	clear();
	attron(COLOR_PAIR(51));
	mvprintw(0,0,"   --MUERTOS--");
	attron(COLOR_PAIR(53));
	mvprintw(0,resultado,"  --DORMIDOS--");
	attron(COLOR_PAIR(52));
	mvprintw(0,resultado*2," --ACTIVOS--");
	attron(COLOR_PAIR(54));
	mvprintw(0,resultado*3,"--EJECUCION--");
	attron(COLOR_PAIR(55));
	mvprintw(0,resultado*4,"--GANADORES--");
	attroff(COLOR_PAIR(55));
	refresh();
	swin=(WINDOW*)malloc(sizeof(WINDOW));
	salert=(WINDOW*)malloc(sizeof(WINDOW));
	bloque=(WINDOW*)malloc(sizeof(WINDOW));
	settearStatus();
	settearPantallaKoopa();
	bool first=true;

	while(mpantalla == true){
		//list_iterate(listaNiveles,(void*)_pantallaNivel);
		nodoNivel* nivel=NULL;
		if(!list_is_empty(listaNiveles)){

			cleanPlanificador();

			if(first){
			first=false;
			nivel=list_get(listaNiveles,0);
			nivel->dibujar = true;
			hiloScroll = hiloGRID(scroller,NULL);
			wrefresh(wind);
			wind=nuevoPanel(STATUS_ROW);
			swin=newwin(4,19,STATUS_ROW+1,cols-35);
			wbkgd(swin,COLOR_PAIR(51)|A_BOLD);
			wattron(swin,COLOR_PAIR(53));
			box(swin, 0, 0);
			salert=newwin(7,30,STATUS_ROW+14,(cols/2)-16);
			wrefresh(salert);
			wbkgd(salert,COLOR_PAIR(54)|A_BOLD);
			//wattron(salert,COLOR_PAIR(54));
			box(salert, 0, 0);
			//wattroff(swin,COLOR_PAIR(54));
			wrefresh(salert);
			wrefresh(wind);
			}
			nivel = list_find(listaNiveles,(void*)_sePuedeDibujar);
			if((nivel==NULL)&&(!list_is_empty(listaNiveles))){
				nivel=list_get(listaNiveles,0);
				nivel->dibujar=true;
			}
			_pantallaNivel(nivel);
		}
		usleep(500000);
//	usleep(200000);
	}
	refresh();
	endwin();
	if(!mnormal)pthread_cancel(hiloScroll);
	pantallaTerminada = true;
	return NULL;
}

bool _sePuedeDibujar(nodoNivel *nivel){
	return nivel->dibujar;
};

void cleanPlanificador(){
	while(!sePuedeBorrar);
	refresh();
	int ancho = cols/2;
	bloque=newwin(4,ancho,STATUS_ROW+1,1);
	wbkgd(bloque,COLOR_PAIR(51));
	wrefresh(bloque);
	ancho = cols - 5;
	bloque=newwin(6,ancho,STATUS_ROW+8,1);
	wbkgd(bloque,COLOR_PAIR(51));
	wrefresh(bloque);
}

WINDOW* nuevoPanel(int posY){
	refresh();
	WINDOW*win=(WINDOW*)malloc(sizeof(WINDOW));
	win=newwin(planiRow,cols-3,posY,0);
	wbkgd(win,COLOR_PAIR(51)|A_BOLD);
	box(win, 0, 0);
	mvwprintw(win,0,1,"Planificador");
	wrefresh(win);
	return win;
};

void _pantallaNivel(nodoNivel*nivel){
	sePuedeBorrar = false;
	if(nivel->dibujar == false)return;
	static int i=0,j,division;
	division=cols/8;
	wattron(wind,COLOR_PAIR(50));
	wrefresh(wind);
	mvwprintw(wind,2,division,"-NOMBRE:%s-",nivel->name);
	mvwprintw(wind,3,division,"-RETARDO:%d-",nivel->algo->retardo);
	for(j=division;j<cols-16;j++)mvwprintw(wind,5,j,"-");
	if (nivel->algo->algo==0){
		mvwprintw(swin,1,1,"-ALGORITMO: SRDF-");
		mvwprintw(swin,2,1," --REM.DIST:%d--",nivel->algo->remainDist);
	}
	else{
		mvwprintw(swin,1,1," -ALGORITMO: RR-");
		mvwprintw(swin,2,1,"  --QUANTUM:%d--",nivel->algo->algo);
	}
	wrefresh(swin);
	wrefresh(wind);
	//attroff(COLOR_PAIR(50));


	void _imprimirPJ(t_player*personaje){
		char recursos[16];
		strcpy(recursos,"[-");
		void _imprimirREC(t_stack*recurso){
			char stack[3];
			stack[0]=recurso->recurso;
			stack[1]='\0';
			strcat(recursos,stack);
			strcat(recursos,"-");
		}
		if(!list_is_empty(personaje->t_stack))list_iterate(personaje->t_stack,(void*)_imprimirREC);
		strcat(recursos,"]");
		wprintw(wind,"-(%c/%c/%s)-",personaje->sym,personaje->data.recsol,recursos);
	}
	wrefresh(wind);
	mvwprintw(wind,7,division,"LISTAS     -(Sym/Rec.Sol/[-Rec.Obt.])-:");
	wattron(wind,COLOR_PAIR(52));
	mvwprintw(wind,9,division,"Listos:    -");
	if(!list_is_empty(nivel->ready))list_iterate(nivel->ready,(void*)_imprimirPJ);
	else wprintw(wind,"-SIN NADIE EN LA LISTA-");
	wprintw(wind,"-");
	wattron(wind,COLOR_PAIR(53));
	mvwprintw(wind,10,division,"Dormidos:  -");
	if(!list_is_empty(nivel->sleeps))list_iterate(nivel->sleeps,(void*)_imprimirPJ);
	else wprintw(wind,"-SIN NADIE EN LA LISTA-");
	wprintw(wind,"-");
	wattron(wind,COLOR_PAIR(51));
	mvwprintw(wind,11,division,"Muertos:   -");
	if(!list_is_empty(nivel->deads))list_iterate(nivel->deads,(void*)_imprimirPJ);
	else wprintw(wind,"-SIN NADIE EN LA LISTA-");
	wprintw(wind,"-");
	wattron(wind,COLOR_PAIR(54));
	mvwprintw(wind,12,division,"EJECUCION: -");
	if(nivel->exe->player!=NULL)_imprimirPJ(nivel->exe->player);
	else wprintw(wind,"-SIN NADIE EN EJECUCION-");
	wprintw(wind,"-");
	wattroff(wind,COLOR_PAIR(54));
	wrefresh(wind);
	if(!list_is_empty(nivel->inters)) interrupcionPlani(nivel);

	if (i>=(list_size(listaNiveles)))i=0;
	sePuedeBorrar = true;
	return;
}

void interrupcionPlani(nodoNivel*nivel){
	int i,j,k,tercera=cols/3;
	int lim=(tercera*2)-13;
	int *aux;
	wrefresh(wind);
	for (j=lim;j<(lim+12);j++)mvwprintw(wind,18,j," ");
	wrefresh(wind);
	for(i=0;i<5;i++){
		wrefresh(wind);
		wattron(wind,COLOR_PAIR(54)|A_BOLD);
		mvwprintw(wind,15,tercera+3,"/");wattron(wind,COLOR_PAIR(57));wprintw(wind,"_");wattron(wind,COLOR_PAIR(54));wprintw(wind,"\\");
		mvwprintw(wind,16,tercera+2,"/");wattron(wind,COLOR_PAIR(57));wprintw(wind,"/ \\");wattron(wind,COLOR_PAIR(54));wprintw(wind,"\\");
		mvwprintw(wind,17,tercera+1,"/");wattron(wind,COLOR_PAIR(57));wprintw(wind,"/   \\");wattron(wind,COLOR_PAIR(54));wprintw(wind,"\\");
		mvwprintw(wind,18,tercera,"/");wattron(wind,COLOR_PAIR(57));wprintw(wind,"/     \\");wattron(wind,COLOR_PAIR(54));wprintw(wind,"\\");
		mvwprintw(wind,19,tercera-1,"/");wattron(wind,COLOR_PAIR(57));wprintw(wind,"/=======\\");wattron(wind,COLOR_PAIR(54));wprintw(wind,"\\");
		wattron(wind,COLOR_PAIR(56)|A_BOLD);
		mvwprintw(wind,16,tercera+4,"|");
		mvwprintw(wind,17,tercera+3," | ");
		mvwprintw(wind,18,tercera+2,"  *  ");
		wattron(wind,COLOR_PAIR(54)|A_BOLD);
		mvwprintw(wind,16,(tercera*2)-13,"INTERRUPCION");
		wattron(wind,COLOR_PAIR(53)|A_BLINK);
		pthread_mutex_lock( &mutexInterr);
		if(!list_is_empty(nivel->inters))aux=(int*)list_remove(nivel->inters,0);
		pthread_mutex_unlock( &mutexInterr);
		switch(*aux){
			case 1: mvwprintw(wind,18,lim," ALGORITMO");
			break;
			case 2: mvwprintw(wind,18,lim,"  RETARDO");
			break;
			case 3: mvwprintw(wind,18,lim,"DESCONEXION");
			break;
			case 4: mvwprintw(wind,18,lim,"   MUERTE");
			break;
			case 5: mvwprintw(wind,18,lim," EXPLOSION");
			break;
		}
		wattroff(wind,COLOR_PAIR(53)|A_BLINK);
		//free(aux);
		wattroff(wind,COLOR_PAIR(54)|A_BOLD);
		wrefresh(wind);
		usleep(100000);
		wrefresh(wind);
		wattron(wind,COLOR_PAIR(54)|A_BOLD);
		for(j=(tercera-1);j<(tercera+27);j++){
			for(k=15;k<20;k++){
				if(k==18){
					if((j<lim)||j>(lim+11))	mvwprintw(wind,k,j," ");
				}else mvwprintw(wind,k,j," ");
			}
		}
		wattroff(wind,COLOR_PAIR(54)|A_BOLD);
		wrefresh(wind);
		usleep(100000);
	}
	wrefresh(wind);
	for (j=lim;j<(lim+12);j++)mvwprintw(wind,18,j," ");
	wrefresh(wind);
}
















