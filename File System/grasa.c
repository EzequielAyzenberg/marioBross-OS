#include "grasa.h"
#include <string.h> //para strCat
#include <sys/mman.h> //biblioteca para usar mmap y sus derivadas
#include <commons/bitarray.c> //para las funciones que manejan el BitMap

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

int main (int argc, char *argv[]){
	
	 
	int fd;
	int bloque,i;
	GHeader* cabeza;
	GFile* nodo;
	ptrGBloque ptrb;
	t_bitarray* bit;
	bool ocupado;
	
	
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
    
    
   
	


	close(fd);

	return 0;
}




int tamanioDelDisco(){
	
	puts("ingrese el tamaño del disco en bytes:");
	return 4096;	
}


int leerHeader(archMap){
	GHeader* cabeza;
	int error;
	
	//prototipo void *mmap(void *addr, size_t length, int prot, int flags,int fd, off_t offset);
	cabeza = (GHeader*) mmap(NULL, BLOQUE, PROT_READ, MAP_SHARED,archMap, NULL);
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
		
		
	nodo = mmap(NULL, DISCO, PROT_READ, MAP_SHARED,archDisk, NULL);
	nodo = nodo+2; //corro 2 posiciones el puntero para que apunte al primer nodo de la tabla
	
	printf("estado: %d \n",nodo[0].state);
	printf("nombre: %s \n",nodo[0].fname);
	printf("padre %d \n",nodo[0].parent_dir_block);
	printf("tamanio %d \n",nodo[0].file_size);
	printf("fecha modificacion %d \n",nodo[0].m_date);
	printf("fecha creacion %d \n",nodo[0].c_date);
	
	
	
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
	
	i=1;
		for (i; i<bitarray_get_max_bit(bit); i++)
		{
		ocupado = bitarray_test_bit(bit,i);
		if (ocupado) printf ("%d) ocupado\n",i);
		if (!ocupado) printf ("%d) desocupado\n",i);
		}
		
		//if ((munmap( bit->bitarray,bit->size ) ) == -1) puts ("fallo el mumapi");
}

