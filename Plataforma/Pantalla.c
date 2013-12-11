
#define STATUS_ROW 7
#define PLANI_ROW 15
#include "Pantalla.h"

int rows,cols;
WINDOW* nuevoPanel(int posY);
void nuevoStatus(WINDOW* statusWin, WINDOW* koopaWin);
void _pantallaNivel(nodoNivel*);

//WINDOW* ppal;
extern bool mpantalla;
extern t_list *listaNiveles;
int resultado;





void *pantalla(void*parametro){
	nivel_gui_get_term_size(&rows,&cols);
	cols--;
	resultado=cols/5;
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

	WINDOW *statusWin=NULL,*koopaWin=NULL;
	nuevoStatus(statusWin,koopaWin);
	while(1){
		list_iterate(listaNiveles,(void*)_pantallaNivel);



		usleep(100000);
	}






	refresh();
	getch();

	endwin();
	return NULL;
}

WINDOW* nuevoPanel(int posY){
	refresh();
	WINDOW*win=(WINDOW*)malloc(sizeof(WINDOW));
	win=newwin(PLANI_ROW,cols,posY,0);
	wbkgd(win,COLOR_PAIR(51));
	box(win, 0, 0);
	mvwprintw(win,0,1,"Planificador");
	wrefresh(win);
	return win;
};

void nuevoStatus(WINDOW* statusWin, WINDOW* koopaWin){
	refresh();
	div_t mitad = div(cols,2);
	int ancho = mitad.quot;
	int alto = STATUS_ROW -2;
	int posY = 1;
	int posX = 0;
	statusWin = newwin(alto,ancho,posY,posX);
	wbkgd(statusWin,COLOR_PAIR(51));
	box(statusWin, 0, 0);
	mvwprintw(statusWin,0,1,"StatusBar");
	wrefresh(statusWin);

	ancho = mitad.quot + mitad.rem;
	posX = mitad.quot;
	koopaWin = newwin(alto,ancho,posY,posX);
	wbkgd(koopaWin,COLOR_PAIR(50));
	box(koopaWin, 0, 0);
	mvwprintw(koopaWin,0,1,"KoopaBar");
	wrefresh(koopaWin);
	return;
};

void _pantallaNivel(nodoNivel*nivel){
	static int i=0, result1,result2;
	result1=cols/3;
	result2=cols/2;
	WINDOW*win=nuevoPanel(STATUS_ROW+(PLANI_ROW*i++));
	wattron(win,COLOR_PAIR(50));
	wrefresh(win);
	mvwprintw(win,1,(result1*0)+1,"\t\t\t--NOMBRE:%s--",nivel->name);
	if (nivel->algo->algo==0)mvwprintw(win,1,result1*1,"\t\t\t\t --ALGORITMO:SRDF--");
	else{
		mvwprintw(win,1,result1*1,"\t\t --ALGORITMO:RR--");
		mvwprintw(win,1,result1*2,"\t\t--CUANTUM:%d--",nivel->algo->algo);
	}
	mvwprintw(win,2,result2*0+1,"\t\t  --REM.DIST:%d--",nivel->algo->remainDist);
	mvwprintw(win,2,result2*1,"   --RETARDO:%d--",nivel->algo->retardo);
	wrefresh(win);
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
		wprintw(win,"-(%c/%c/%s)-",personaje->sym,personaje->data.recsol,recursos);
	}
	wrefresh(win);
	mvwprintw(win,3,2,"\t\t\t  LISTAS -(Sym/Rec.Sol/[-Rec.Obt.])-:");
	wattron(win,COLOR_PAIR(52));
	mvwprintw(win,5,2,"\t\t\t  Listos: -");
	if(!list_is_empty(nivel->ready))list_iterate(nivel->ready,(void*)_imprimirPJ);
	wprintw(win,"-");
	wattron(win,COLOR_PAIR(53));
	mvwprintw(win,6,2,"\t\t\t  Dormidos: -");
	if(!list_is_empty(nivel->sleeps))list_iterate(nivel->sleeps,(void*)_imprimirPJ);
	wprintw(win,"-");
	wattron(win,COLOR_PAIR(51));
	mvwprintw(win,7,2,"\t\t\t  Muertos: -");
	if(!list_is_empty(nivel->deads))list_iterate(nivel->deads,(void*)_imprimirPJ);
	wprintw(win,"-");
	wattron(win,COLOR_PAIR(54));
	mvwprintw(win,8,2,"\t\t\t  EJECUCION: -");
	if(nivel->exe->player!=NULL)_imprimirPJ(nivel->exe->player);
	wprintw(win,"-");
	wattroff(win,COLOR_PAIR(54));
	wrefresh(win);




	if (i>=(list_size(listaNiveles)))i=0;
}




















