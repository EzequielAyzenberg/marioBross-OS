#include "grasa.h"
#include <string.h> //para strCat
#include <sys/mman.h> //biblioteca para usar mmap y sus derivadas
#include <commons/bitarray.c> //para las funciones que manejan el BitMap
#include <commons/collections/list.c>

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

/*
void imprimir(void* arch){
	
	printf("%s\n",(char*)arch);
}
*/
/*
int main (int argc, char *argv[]){
	
	 
	int fd;
	int bloque,i;
	GHeader* cabeza;
	GFile* nodo;
	ptrGBloque ptrb;
	t_bitarray* bit;
	t_list* lista = list_create();
	
	
	
	//apertura del Disco
	if (argc != 2) {
		perror("error en las cantidad de argumentos");
		return EXIT_FAILURE;
	}
	else {//si pasaron la cantidad de parametro correctos abrimos el archivo
		 fd = open(argv[1], O_RDONLY);
           if (fd == -1) printf("error al abrir al archivo binario");
    }
    
    
    leerHeader(fd); //Muestras el header MAN!
    
    bitArray(fd);  //bitArray is de new sensation
    
    tablaDeNodos(fd); //manejo de nodos
    
    nodo = mmap(NULL, DISCO, PROT_READ, MAP_SHARED,fd, BLOQUE*2);
    queHayAca(nodo, 0,lista);
	list_iterate(lista, imprimir);
	printf("tamanio lista: %d \n",list_size(lista));
	


	close(fd);

	return 0;
}
*/





int tamanioDelDisco(){
	
	puts("ingrese el tamaño del disco en bytes:");
	return 4096;	
}


int leerHeader(archDisk){
	GHeader* cabeza;
	
	//prototipo void *mmap(void *addr, size_t length, int prot, int flags,int fd, off_t offset);
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

int tablaDeNodos(int archDisk){
	
	//Var Locales
	GFile* nodo;
	int i;
	
		
		
	nodo = (GFile*)mmap(NULL, DISCO, PROT_READ, MAP_SHARED,archDisk, BLOQUE*2);
	
	printf("estado: %d \n",nodo[0].state);
	printf("nombre: %s \n",nodo[0].fname);
	printf("padre %d \n",nodo[0].parent_dir_block);
	printf("tamanio %d \n",nodo[0].file_size);
	printf("fecha modificacion %d \n",(int)nodo[0].m_date);
	printf("fecha creacion %d \n",(int)nodo[0].c_date);
	
	for (i=0; i < 1024;i++)
		 if ((nodo[i].parent_dir_block == nodo[0].parent_dir_block)&&(nodo[i].state!=0)) printf ("nombre: %s\n",nodo[i].fname);
	
	//if ((munmap( nodo,DISCO ) ) == -1) puts ("fallo el mumapi");
	return 0;
}


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
	
			
		//if ((munmap( bit->bitarray,bit->size ) ) == -1) puts ("fallo el mumapi");
	return 0;
}

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
