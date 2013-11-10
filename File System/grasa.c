#include "grasa.h"
#include <sys/mman.h> //biblioteca para usar mmap y sus derivadas
#include <commons/bitarray.c> //para las funciones que manejan el BitMap
#include <commons/collections/list.c> //para devolver listas en queHayAca
#include <commons/string.h>//para usar split en how

#define BLOQUE 4096
#define DISCO 10485760



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


int nodoByPath(const char* path,GFile* nodo){
	char** nombreHijo;
	int i;
	
	int j;
	j=0;
	int s;
	s=0;
	int numPadre;
	
	
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
	
	//if(!string_equals_ignore_case(nodo[numPadre].fname,nombreHijo[s-1])) numPadre = -1;
	
	
	return numPadre;
}
	

	
	

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


