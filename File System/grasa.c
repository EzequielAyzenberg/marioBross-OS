#include "grasa.h"
#include <sys/mman.h> //biblioteca para usar mmap y sus derivadas
#include <commons/bitarray.c> //para las funciones que manejan el BitMap
#include <commons/collections/list.c> //para devolver listas en queHayAca
#include <commons/string.h>//para usar split en how

#define BLOQUE 4096
#define DISCO 10485760
#define MAXNODO 1023
#define FAIL -1




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
void* dir_bloque(int n);
void left_strcat(char*destino,char*origen);
uint8_t* bloqueDondeEstaElByte(GFile* nodo,int byte);

/* funcion direccion bloque
void* dir_bloque(int n){
	return ptr_mmap + BLOCK_SIZE*n;	
}
* */
int tamanioDelDisco(){
	
	puts("ingrese el tamaño del disco en bytes:");
	return 4096;	
}

/*
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
*/
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
	

	bit = bitarray_create(dir_block(header->blk_bitmap), 320);
	
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

/*
 * A partir del path consigue el inodo correspondiente
 */
int nodoByPath(const char* path,GFile* nodo){
	char** nombreHijo;
	int numNodo;
	int acierto;
	acierto=0;
	int numHijo;
	numHijo=0;
	int encontrado;
	
	int numPadre;
	
	
	
	nombreHijo = string_split((char*)path,"/");
	
	
	numPadre=0;
	while (nombreHijo[numHijo]!=NULL){
		numNodo=1;	
		encontrado = 0;
		while(numNodo<=MAXNODO){
			if(string_equals_ignore_case(nombreHijo[numHijo],nodo[numNodo].fname)) acierto++; 
			if((numPadre==nodo[numNodo].parent_dir_block)) acierto++;
			if (acierto==2) {numPadre=numNodo; encontrado=1;}
			acierto=0;
			numNodo++;
		
		}
			
		numHijo++;
	}
	
	
	if(strcmp(path, "/") == 0) encontrado=1;
	
	
	if(!encontrado) return FAIL;    
	else return numPadre;
}

int readGrid(char *buf, size_t size, off_t offset,GFile* nodo){
	
	int bytesLeidos;
	int byteRestantes;
	int bytePedido;
	int bytesParaCopiar;
	uint8_t* ptr_datos;
	
	printf("soy el nodo: %s dentro de cargar buffer \n",nodo[0].fname);
	printf("cuanto es el size: %d \n",(int)size);
	printf("cuanto es el offset: %d \n",offset);
	
	bytePedido=offset;
	bytesLeidos=0;
	
while (bytesLeidos < size) {
	puts("entre al while de memcpy");
   ptr_datos = bloqueDondeEstaElByte(nodo,(int) bytePedido);
   bool esElPrimero = bytesLeidos == 0;  
   int offsetDentroDelBloque = esElPrimero ? offset % BLOQUE : 0;
   int bytesRestantesDentroDelBloque = BLOQUE - offsetDentroDelBloque;

   int bytesParaCopiar = size - bytesLeidos;
   if (bytesParaCopiar > bytesRestantesDentroDelBloque)
      bytesParaCopiar = bytesRestantesDentroDelBloque;
	
	
   memcpy(buf + bytesLeidos, ptr_datos + offsetDentroDelBloque, bytesParaCopiar);
   bytesLeidos += bytesParaCopiar;
   bytePedido += bytesParaCopiar;
   
	}

	
}


uint8_t* bloqueDondeEstaElByte(GFile* nodo,int byte){
	
	int numeroBloqueIndirecto;
	int numeroBloqueDato;
	int dirBloqueArray;
	int offsetDelArray;
	ptrGBloque* ptrBloque;
	
	
	
	numeroBloqueDato = byte/BLOQUE; //desde cual bloque de datos voy a leer
	numeroBloqueIndirecto = numeroBloqueDato/1024;  //si el bloque de datos es mayor a 1024 ya pertenece al indirecto de la pocision 1
	offsetDelArray = numeroBloqueDato%1024;
	dirBloqueArray=nodo->blk_indirect[numeroBloqueIndirecto]; //tengo la direcion del bloque de array con puntero a bloques de datos
	printf("tengo la direcion del bloque de array con puntero a bloques de datos: %d \n",dirBloqueArray);
	ptrBloque =(ptrGBloque*) dir_bloque(dirBloqueArray); //ahora estoy apuntando a la primera posicion del bloque de array con punteros a bloque de datos
	printf("ahora estoy apuntando a la primera posicion del bloque de array con punteros a bloque de datos: %d \n",*ptrBloque);
	ptrBloque=ptrBloque+offsetDelArray;         //ahora estoy apundo a la direccion del bloque que quiero leer
	printf("ahora estoy apundo a la direccion del bloque que quiero leer: %d \n",*ptrBloque);
	return (uint8_t*)dir_bloque(*ptrBloque);   //ahora estoy AL FIN estoy apuntando al la primer posicion del bloque de datos que tengo que leer
	

}

/*	
int crearDirectorio(const char* path,GFile* inodo){
	int numNodo;
	char** subDirectorios;
	int i;
	i=0;
	
	numNodo=nodoByPath(path,inodo);
	if (numNodo!=FAIL) return -EEXIST;
	
	else
	
	subDirectorios = string_split(path,"/");
	while (subDirectorios[i])i++;        //cuento los subdirectorios     
	char *string_substring_until(char *text, int length)
	
	
}
*/
int hijoDondeEstas(char* nombreHijo,int padre,GFile*nodo){
	int i;
	i=1;
	int j;
	j=0;
	while(1){
	if(string_equals_ignore_case(nombreHijo,nodo[i].fname)) j++; 
	if((padre==nodo[i].parent_dir_block)) j++;
    if (j==2)break;
    j=0;
    i++;
  }
    return  i;
	
}

int nodoQueQuiero (const char* path,GFile* nodo){
		int i=1;
	
	
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


