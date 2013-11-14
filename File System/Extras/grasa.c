#include "grasa.h"
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

void left_strcat(char*destino,char*origen);


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
	
	
return 0;
}

/*
int tablaDeNodos(int archDisk){
	
	//Var Locales
	GFile* nodo;
	ptrGBloque i;
	char path[100];

		
	//path = string_new();
	nodo = (GFile*) mmap(NULL, DISCO, PROT_READ, MAP_SHARED,archDisk, BLOQUE);
	strcpy(path,"/dir1/hola.txt");
	printf("cual es el path %s\n",path);
	i=nodoByPath(path,nodo);
	//i=hijoDondeEstas("dir1",0,nodo);
	printf("%d num",i);
	
	printf("estado: %d \n",nodo[i].state);
	printf("nombre: %s \n",nodo[i].fname);
	printf("padre %d \n",nodo[i].parent_dir_block);
	printf("tamanio %d \n",nodo[i].file_size);
	printf("fecha modificacion %d \n",(int)nodo[i].m_date);
	printf("fecha creacion %d \n",(int)nodo[i].c_date);
	
	return 0;
}
*/
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

int queHayAca(char* path,int archDisk,t_list* lista){
	int i;
	int dirPadre;
	GFile* nodo;
	nodo = mmap(NULL, DISCO, PROT_READ, MAP_SHARED,archDisk, BLOQUE);

	if (strcmp(path,"/")==0) dirPadre=0;
	//else dirPadre=nodoByPath((char*) path,nodo);

	for (i=1; i < 1023;i++)
				if ((dirPadre == nodo[i].parent_dir_block)&&(nodo[i].state!=0)) list_add(lista, nodo[i].fname);
return 0;
}


int nodoByPath(const char* path,int fd){
	char** nombreHijo;
	char* newPath;
	int i;
	i=0;
	int numPadre;
	int numHijo;
	GFile* nodo;
	nodo = (GFile*)mmap(NULL, DISCO, PROT_READ, MAP_SHARED,fd, BLOQUE);
	newPath=string_duplicate((char*)path);
	
    nombreHijo = string_split(newPath,"/");
	numPadre=0;
	
	while (nombreHijo[i]) {    
		numHijo = hijoDondeEstas(nombreHijo[i],numPadre,nodo);
	    numPadre=numHijo;
	    i++;
	}
	
	return numPadre;
}


int hijoDondeEstas(char* nombreHijo,int padre,GFile*nodo){
	int i;
	i=0;
	while ((strcmp(nombreHijo,(const char*)nodo[i].fname))|| (padre!=nodo[i].parent_dir_block)) i++;
   
  
    return  i;
	
}

int nodoQueQuiero (const char* path,GFile* nodo){
		int i=0;
	
	
	while(i<1024){
		char barra[72];
		strcpy(barra,nodo[i].fname);
		left_strcat(barra,"/");
		if (strcmp(path,barra) == 0) break;
		i++;
		}
	
	
	return i;
}

void left_strcat(char*destino,char*origen){
	char base[72];
	strcpy(base,origen);
	strcat(base,destino);
	strcpy(destino,base);
}


