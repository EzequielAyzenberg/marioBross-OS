
#define STATUS_ROW 7
#define PLANI_ROW 20
#include "Pantalla.h"

int rows,cols;
WINDOW* nuevoPanel(int posY);
void nuevoStatus(WINDOW* statusWin, WINDOW* koopaWin);
void _pantallaNivel(nodoNivel*,WINDOW*,WINDOW*);
void interrupcionPlani(WINDOW*);
void dibujarBarra(void);


//WINDOW* ppal;
extern bool mpantalla;
extern t_list *listaNiveles;
int resultado;





void *pantalla(void*parametro){
	nivel_gui_get_term_size(&rows,&cols);
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
	init_pair(56,COLOR_BLACK,COLOR_WHITE);
	init_pair(57,COLOR_RED,COLOR_RED);
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
	WINDOW*wind,*swin=(WINDOW*)malloc(sizeof(WINDOW));;
	nuevoStatus(statusWin,koopaWin);
	bool first=true;

	while(1){
		//list_iterate(listaNiveles,(void*)_pantallaNivel);
		nodoNivel* nivel=NULL;
		int indice=0;
		if(!list_is_empty(listaNiveles)){
			if(first){
				wrefresh(wind);
				mvwprintw(wind,20,3,"PRIMERO");
				wind=nuevoPanel(STATUS_ROW);
				mvwprintw(wind,20,3,"SEGUNDO");
				swin=newwin(4,19,STATUS_ROW+1,cols-35);
				wbkgd(swin,COLOR_PAIR(51)|A_BOLD);
				wattron(swin,COLOR_PAIR(53));
				box(swin, 0, 0);
				wattroff(wind,COLOR_PAIR(53));
				wrefresh(wind);
				first=false;
			}
			do{
				wrefresh(wind);
				mvwprintw(wind,20,3,"TERCERO");
				nivel=(nodoNivel*)list_get(listaNiveles,indice);
				mvwprintw(wind,20,3,"CUARTO ");
				indice++;
				wrefresh(wind);
			}while((!(nivel->dibujar))||list_size(listaNiveles)>=indice);
			if(nivel!=NULL)_pantallaNivel(nivel,wind,swin);
			dibujarBarra();
		}
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
	win=newwin(PLANI_ROW,cols-3,posY,0);
	wbkgd(win,COLOR_PAIR(51)|A_BOLD);
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

void _pantallaNivel(nodoNivel*nivel,WINDOW*wind,WINDOW*swin){
	static int i=0,j,division;
	division=cols/5;
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

	interrupcionPlani(wind);

	if (i>=(list_size(listaNiveles)))i=0;
}
 void dibujarBarra(void){

 }




void interrupcionPlani(WINDOW*wind){
	wrefresh(wind);
	wattron(wind,COLOR_PAIR(54)|A_BOLD);
	mvwprintw(wind,3,9,"_");
	mvwprintw(wind,4,8,"/");wattron(wind,COLOR_PAIR(57));wprintw(wind,"_");wattron(wind,COLOR_PAIR(54));wprintw(wind,"\\");
	mvwprintw(wind,5,7,"/");wattron(wind,COLOR_PAIR(57));wprintw(wind,"/ \\");wattron(wind,COLOR_PAIR(54));wprintw(wind,"\\");
	mvwprintw(wind,6,6,"/");wattron(wind,COLOR_PAIR(57));wprintw(wind,"/   \\");wattron(wind,COLOR_PAIR(54));wprintw(wind,"\\");
	mvwprintw(wind,7,5,"/");wattron(wind,COLOR_PAIR(57));wprintw(wind,"/     \\");wattron(wind,COLOR_PAIR(54));wprintw(wind,"\\");
	mvwprintw(wind,8,4,"/");wattron(wind,COLOR_PAIR(57));wprintw(wind,"/=======\\");wattron(wind,COLOR_PAIR(54));wprintw(wind,"\\");
	wattron(wind,COLOR_PAIR(56)|A_BOLD);
	mvwprintw(wind,5,9,"|");
	mvwprintw(wind,6,8," | ");
	mvwprintw(wind,7,7,"  *  ");
	wattron(wind,COLOR_PAIR(54)|A_BOLD);
	mvwprintw(wind,10,3,"INTERRUPCION");
	wattroff(wind,COLOR_PAIR(54)|A_BOLD);
	wrefresh(wind);
}


/*

              _
			 /_\
			// \\
		   // | \\
		  //  *  \\
		 //=======\\

*/















