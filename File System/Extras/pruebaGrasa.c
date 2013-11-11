#include "grasa.h"
//#include <string.h> //para strcat
#include <sys/mman.h> //biblioteca para usar mmap y sus derivadas
#include <commons/bitarray.c> //para las funciones que manejan el BitMap
#include <commons/collections/list.c> //para devolver listas en queHayAca
#include <commons/string.h>//para usar split en how

#define BLOQUE 4096
#define DISCO 10485760
#define correc 2


/*datos recopilados:
usar funcion open(...)    hecho
usar funcion mmap         hecho     
pasarla al fuse la funcion o la estructura para correcta a la funcion correcta para que haga su magia
    -el ptrGblock contiene un numero fisico que representa un bloque
    -ese bloque tiene un array de direcciones a bloque que valla a saber Dios como se los voy a dar al fuse
ver que onda el flag MAP_SHARED
agregar otra funcionalidad ademas de read con un pipe
*/

//disco de prueba tiene 6.147.455 bytes comprimido  10 mb sin comprimir 10240
GFile* ptr_nodo;
uint8_t* ptr_mmap;
GHeader* ptr_header;
t_bitarray* bitMap;

/*
void imprimir(void* arch){
	
	printf("%s\n",(char*)arch);
}
*/
/*
int nodoByPath(const char* path,GFile*);
int hijoDondeEstas(char* path,int padre,GFile*);
int tablaDeNodos(int archDisk);
*/
void* dir_bloque(int n);
//char** dameLosNombresDelPath(char* );

int main (int argc, char *argv[]){
	
	 
	int fd;
    
	int bloque,i;
	extern uint8_t* ptr_mmap;
	extern GHeader* ptr_header;
	extern GFile* ptr_nodo;
	ptrGBloque ptrb;
	char** nombre;
	char* path;
	t_bitarray* bit;
	//t_list* lista = list_create();
	
	
	

	//apertura del Disco
	if (argc != 2) {
		perror("error en las cantidad de argumentos");
		return EXIT_FAILURE;
	}
	else {//si pasaron la cantidad de parametro correctos abrimos el archivo
		 fd = open(argv[1], O_RDONLY);
           if (fd == -1) printf("error al abrir al archivo binario");
    }
    ptr_mmap = mmap(NULL, DISCO, PROT_READ, MAP_SHARED,fd, NULL);
    ptr_header = (GHeader*) dir_bloque(0);
    ptr_nodo = (GFile*) dir_bloque(ptr_header->size_bitmap);
    
  //  leerHeader(fd); //Muestras el header MAN!
    
  //  bitArray(fd);  //bitArray is de new sensation
    
    tablaDeNodos(); //manejo de nodos
   
   
    
    
    //queHayAca(nodo, 0,lista);
	//list_iterate(lista, imprimir);
	//printf("tamanio lista: %d \n",list_size(lista));
	


    
    
	close(fd);

	return 0;
}


void* dir_bloque(int n){
	extern uint8_t* ptr_mmap;
	return ptr_mmap + BLOQUE*n;	
}





int tamanioDelDisco(){
	
	puts("ingrese el tamaño del disco en bytes:");
	return 4096;	
}


int leerHeader(archDisk){
	GHeader* cabeza;
	
	//prototipo void *mmapnew(void *addr, size_t length, int prot, int flags,int fd, off_t offset);
	cabeza = (GHeader*) mmap(NULL, BLOQUE, PROT_READ, MAP_SHARED,archDisk,(off_t)NULL);
	if (cabeza == MAP_FAILED) puts("fallo el mapeo pues");
	
	printf("que tengo en la cabeza? %s \n",cabeza->grasa);
	printf("version: %d \n",cabeza->version);
	printf("bloque de inicio: %d \n",cabeza->blk_bitmap);
	printf("tamanio del bitmap en bloques %d \n",cabeza->size_bitmap);
	
	
	//int munmap(void *addr, size_t len);
	//if ((munmap( cabeza,BLOQUE ) ) == -1) puts ("fallo el mumapi"); se los debo por ahora
	
return 0;
}


int tablaDeNodos(){
	
	//Var Locales
	ptrGBloque i;
	extern GFile* ptr_nodo;
	int numpadre;
	char path[1000] = "/dir1/hola.txt";
	char* newPath;
	char* nombre;
	char* buffer;
    
		
	//path = string_new();
	//nodo = (GFile*) mmap(NULL, DISCO, PROT_READ, MAP_SHARED,archDisk, BLOQUE);
	//strcpy(path,"/dir1/hola.txt");
	//strcpy(path,"/dir1/secret");
	//strcpy(path,"/otro_dir");
	//strcpy(path,"/ar");
	
	//printf("cual es el path %s\n",newPath);
	
	//printf("total de / \n%d",strspn(path,"/"));
	
	
	//printf("desaparecio / ? %s\n",*(string_split("/dir1","/")));
	
	
	t_list* archivos;
	archivos = list_create();
	numpadre=nodoByPath(path,ptr_nodo);
	queHayAca(numpadre,ptr_nodo,archivos);
		

	for (i=0; i < list_size(archivos); i++){
	nombre =(char*)list_get(archivos,i);
	printf("que hay en path%s: %s\n",path,nombre);
	}

list_destroy(archivos);
	
	i=nodoByPath(path,ptr_nodo);
	//i=hijoDondeEstas("dir1",0,nodo);
	printf("%d num",i);
	
	printf("estado: %d \n",ptr_nodo[i].state);
	printf("nombre: %s \n",ptr_nodo[i].fname);
	printf("padre %d \n",ptr_nodo[i].parent_dir_block);
	printf("tamanio %d \n",ptr_nodo[i].file_size);
	printf("fecha modificacion %d \n",(int)ptr_nodo[i].m_date);
	printf("fecha creacion %d \n",(int)ptr_nodo[i].c_date);
	printf("bloque de indireccion numero: %d \n", (int)ptr_nodo[i].blk_indirect[0]);
	printf("bloque de indireccion numero: %d \n", (int)ptr_nodo[i].blk_indirect[1]);
	
	cargarBuffer(buffer,BLOQUE*10-2, (off_t)4096*2+17,ptr_nodo[i]);
	
	
	return 0;
}

/*
int bitArray(archDisk){
	t_bitarray* bit;
	bool ocupado;
	int i;
	

	bit = bitarray_create(bit->bitarray, 320);
	bit->bitarray = mmap(NULL, BLOQUE, PROT_READ, MAP_SHARED,archDisk, BLOQUE);
	
	printf("tamaño del bitmap %d",bitarray_get_max_bit(bit));
	
		for (i=1; i<bitarray_get_max_bit(bit); i++)
		{
		ocupado = bitarray_test_bit(bit,i);
		if (ocupado) printf ("%d) ocupado\n",i);
		if (!ocupado) printf ("%d) desocupado\n",i);
		}
	
	bitarray_destroy(bit);		
		//if ((munmap( bit->bitarray,bit->size ) ) == -1) puts ("fallo el mumapi");
	return 0;
}
*/


int queHayAca(int numNodo,GFile* nodo,t_list* lista){
	int i;
	int dirPadre;
	dirPadre=numNodo;
	//if (strcmp(path,"/")==0) dirPadre=0;
	//else dirPadre=nodoByPath((path,nodo);

	for (i=1; i < 1023;i++)
				if ((dirPadre == nodo[i].parent_dir_block)&&(nodo[i].state!=0)) list_add(lista, nodo[i].fname);
return 0;
}


int nodoByPath(const char* path,GFile* nodo){
	char** nombreHijo;
	int i;
	
	int j;
	j=0;
	int s;
	s=0;
	int numPadre;
	int numHijo;
	
	nombreHijo = string_split((char*)path,"/");
	
	
	numPadre=0;
	while (nombreHijo[s]!=NULL){
	i=1;	
	while(i<1023){
	if(string_equals_ignore_case(nombreHijo[s],nodo[i].fname)) j++; 
	if((numPadre==nodo[i].parent_dir_block)) j++;
    if (j==2) numPadre=i;
    j=0;
    i++;
    
		}
		
	s++;
}
	

	
	
	return numPadre;
}

int cargarBuffer(char *buf, size_t size, off_t offset,GFile* inodo){
	
	int hastaCualBlk_indirectLeo;
	int desdeCualBlk_indirectLeo;
	int hastaCualBlk_directLeo;
	int deCualBlK_directLeo;
	int offsetDelPrimero;
	int cuantoLeoDelUltimo;
	
	
	deCualBlK_directLeo = offset/BLOQUE; //desde cual bloque de datos voy a leer
		
	hastaCualBlk_directLeo = (size+offset)/BLOQUE; //hasta cual bloque de datos voy a leer
	if(((size+offset)%BLOQUE)>0) hastaCualBlk_directLeo *= 1;
		
	offsetDelPrimero = offset%BLOQUE;
	
	cuantoLeoDelUltimo = (size + offset)%BLOQUE;
	
	desdeCualBlk_indirectLeo = deCualBlK_directLeo/1024;
			   
	hastaCualBlk_indirectLeo = hastaCualBlk_directLeo/1024; //si da 0 es el primero
	//if((hastaCualBlk_indirectLeo%1024)>0) hastaCualBlk_indirectLeo *= 1;
	
	
	 
	
	printf("cuantosBlk_indirectLeo: %d \n",hastaCualBlk_indirectLeo);
	printf("deCualBlk_indirectEmpiezo: %d \n",desdeCualBlk_indirectLeo);
	printf("cuantosBlk_directLeo: %d \n",hastaCualBlk_directLeo);
	printf("deCualBlk_directEmpiezo: %d \n",deCualBlK_directLeo);
	printf("cuanto leo del ultimo: %d \n",cuantoLeoDelUltimo);
	printf("offset del primero: %d \n",offsetDelPrimero);
	
	
	
}









/*
int queHayAca(const char* path,int archDisk,t_list* lista){
	int i;
	int dirPadre;
	GFile* nodo;
	nodo = mmap(NULL, DISCO, PROT_READ, MAP_SHARED,archDisk, BLOQUE);

	if (strcmp(path,"/")==0) dirPadre=0;
	else {dirPadre=nodoByPath((char*) path,nodo);puts("me ejecute");printf("dirpadre%d",dirPadre);}

	for (i=1; i < 1024;i++)
				if ((dirPadre == nodo[i].parent_dir_block)&&(nodo[i].state!=0)) list_add(lista, nodo[i].fname);
return 0;
}
*/
/*
int queHayAca(char* path,int archDisk,t_list* lista){
	int i;
	int dirPadre;
	GFile* nodo;
	nodo = mmap(NULL, DISCO, PROT_READ, MAP_SHARED,archDisk, BLOQUE*2);
	
	if (strcmp(path,"/")==0) dirPadre=0;
	for (i=0; i < 1023;i++)
		 if ((dirPadre == nodo[i].parent_dir_block)&&(nodo[i].state!=0)) list_add(lista, nodo[i].fname);
	
return 1;	
}
*/
/*
void left_strcat(char*destino,char*origen){
	char base[72];
	strcpy(base,origen);
	strcat(base,destino);
	strcpy(destino,base);
}

char* path_padre(uint32_t padre,GFile*nodo){
	char*temp;
	if(padre==0){
		return "/";
	}
	strcpy(temp,(char*)nodo[padre-correc].fname);
	strcat(temp,"/");
	left_strcat(temp,path_padre(nodo[padre-correc].parent_dir_block,nodo));
	return temp;
}
*/
/*
int nodoByPath(const char* path,GFile* nodo){
	char** nombreHijo;
	char* newPath;
	int i;
//	int casos;
	i=0;
	int numPadre;
	int numHijo;
	//printf("cual es el path %s \n",path);
	//nombreHijo = dameLosNombresDelPath(path);
	//newPath=string_new();
	newPath=string_duplicate((char*)path);
	//newPath=string_substring_from(path, 1);
	//printf("la ruta sin barra: %s\n",newPath);
	//printf("nombre hijo %s \n",nombreHijo[0]);
	nombreHijo = string_split(newPath,"/");
	//printf("%s \n",nombreHijo[0]);
	//printf("%s \n",nombreHijo[1]);
	
	//if(strlen(nombreHijo[1]))printf("nombre hijo %s \n",nombreHijo[1]);
	//numHijo = hijoDondeEstas(nombreHijo[0],0,nodo);
	numPadre=0;
	//casos = newStrlen(nombreHijo);
	//printf("casos %d",casos);
	while (nombreHijo[i]) {     //aca me tira el segmatacion fault
		numHijo = hijoDondeEstas(nombreHijo[i],numPadre,nodo);
	    numPadre=numHijo;
	    i++;
	}
	
	return numPadre;
}

/*
char** dameLosNombresDelPath(char* path){
	char* nombreNuevo;
	char** nombres;
	nombreNuevo=string_new();
	nombres=string_split(path,"/");
	if(strlen(nombres[1]))printf("nombre hijo%s \n",nombres[1]);
    memcpy(nombreNuevo,nombres[newStrlen(nombres)-1],strlen(nombres[newStrlen(nombres)-1])-1);
    strcpy(nombres[newStrlen(nombres)-1],nombreNuevo);

return nombres;	
}	
*/
/*
int newStrlen(char**str){
	
	int i;
	i=0;
	while(1){
		if(strlen(str[i])) break;
		i++;
	}
	return i;
}
*/


/*
int hijoDondeEstas(char* nombreHijo,int padre,GFile*nodo){
	int i;
	i=1;
	while ((strcmp(nombreHijo,(const char*)nodo[i].fname))|| (padre!=nodo[i].parent_dir_block)) i++;
    //                (!(nodo1,dir)=0)=1      &&        (nodo1=0 != 0)=0
    //                   nodo4,secret=0
    return  i;
	
}
*/

/*
 * casos de prueba un rato

	
	if(strlen(nombreHijo[1]))printf("nombre hijo%s \n",nombreHijo[1]);
	if(nombreHijo[2])printf("vedadero \n");
	else printf("falso \n");
	
	*/
