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


int cargarBuffer(char *buf, size_t size, off_t offset,GFile* inodo){
	
	int hastaCualBlk_indirectLeo;
	int desdeCualBlk_indirectLeo;
	int hastaCualBlk_directLeo;
	int deCualBlk_directLeo;
	int offsetDelPrimero;
	int cuantoLeoDelUltimo;
	int dirBloqueArray;
	int offsetBuff;
	ptrGBloque* ptrBloque;
	int primero;
	uint8_t* ptr_datos;
	
	deCualBlk_directLeo = offset/BLOQUE; //desde cual bloque de datos voy a leer
		
	hastaCualBlk_directLeo = (size+offset)/BLOQUE; //hasta cual bloque de datos voy a leer
	if(((size+offset)%BLOQUE)>0) hastaCualBlk_directLeo *= 1;
		
	offsetDelPrimero = offset%BLOQUE;
	
	cuantoLeoDelUltimo = (size + offset)%BLOQUE;
	
	desdeCualBlk_indirectLeo = deCualBlk_directLeo/1024;
			   
	hastaCualBlk_indirectLeo = hastaCualBlk_directLeo/1024; //si da 0 es el primero
	//if((hastaCualBlk_indirectLeo%1024)>0) hastaCualBlk_indirectLeo *= 1;
	
	
	 
	
	printf("cuantosBlk_indirectLeo: %d \n",hastaCualBlk_indirectLeo);
	printf("deCualBlk_indirectEmpiezo: %d \n",desdeCualBlk_indirectLeo);
	printf("cuantosBlk_directLeo: %d \n",hastaCualBlk_directLeo);
	printf("deCualBlk_directEmpiezo: %d \n",deCualBlk_directLeo);
	printf("cuanto leo del ultimo: %d \n",cuantoLeoDelUltimo);
	printf("offset del primero: %d \n",offsetDelPrimero);
	
	buf=malloc(size);
	

	primero=1;
	dirBloqueArray=inodo->blk_indirect[desdeCualBlk_indirectLeo]; //tengo la direcion del bloque de array con puntero a bloques de datos
	ptrBloque =(ptrGBloque*) dir_bloque(dirBloqueArray); //ahora estoy apuntando a la primera posicion del bloque de array con punteros a bloque de datos
	ptrBloque=ptrBloque+deCualBlk_directLeo;         //ahora estoy apundo a la direccion del bloque que quiero leer
	ptr_datos = (uint8_t*)dir_bloque(*ptrBloque);   //ahora estoy AL FIN estoy apuntando al la primer posicion del bloque de datos que tengo que leer
	ptrBloque+=1;	                                //ahora apuntado a la proxima direccion del bloque de datos que tengo que leer
	//primer lectura, el offset
	memcpy(buf,ptr_datos+offsetDelPrimero,BLOQUE-offsetDelPrimero);   //copio los primero datos, aquellos cuyos podrian tener offset De Bloque
	offsetBuff=BLOQUE-offsetDelPrimero;     //actulizo el offset del buff para que no se sobresquiban los datos
	
	//esto es por si hay intermedios
	do{
			
		
			while((deCualBlk_directLeo<hastaCualBlk_directLeo)||((deCualBlk_directLeo%1024)==0)){  //sin desde es menor que hasta entro y si no estoy apuntando al ultimo bloque 
			deCualBlk_directLeo+=1;                         //actualizo desde donde debo leer
			ptr_datos = (uint8_t*)dir_bloque(*ptrBloque);   //ahora estoy apuntando a la primer posicion de memoria del bloque de datos que quiero cargar en el buffer
			memcpy(buf+offsetBuff,ptr_datos,BLOQUE); 		//copio los datos
			offsetBuff+=BLOQUE;  						    //actulizo el offset del Bloque
			ptrBloque+=1;                                   //ahora estoy apundo a la direccion del bloque que quiero leer
			}
		desdeCualBlk_indirectLeo+=1;						//actulizo desde donde leo los directos
		if(!(inodo->blk_indirect[desdeCualBlk_indirectLeo])) //si el proximo indirecto es no es 0
		{
		dirBloqueArray=inodo->blk_indirect[desdeCualBlk_indirectLeo];
	    ptrBloque =(ptrGBloque*) dir_bloque(dirBloqueArray);
	    }
		
	}while(desdeCualBlk_indirectLeo<hastaCualBlk_indirectLeo);
	
	//Esto es para guardar en el buff lo que queda del ultimo bloque, si es que queda
	if(cuantoLeoDelUltimo) 
	{
	ptr_datos = (uint8_t*)dir_bloque(*ptrBloque);   //ahora estoy apuntando a la primer posicion de memoria del bloque de datos que quiero cargar en el buffer
	memcpy(buf+offsetBuff,ptr_datos,cuantoLeoDelUltimo); 		//copio los datos que faltan del ultimo bloque
	}
	
	return 0;
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


