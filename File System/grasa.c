#include "grasa.h"

#include <sys/mman.h> //biblioteca para usar mmap y sus derivadas

#define BLOQUE 4096

/*datos recopilados:
usar funcion open(...)    hecho
usar funcion mmap         hecho     
pasarla al fuse la funcion o la estructura para correcta a la funcion correcta para que haga su magia
    -el ptrGblock contiene un numero fisico que representa un bloque
    -ese bloque tiene un array de direcciones a bloque que valla a saber Dios como se los voy a dar al fuse
ver que onda el flag MAP_SHARED
agregar otra funcionalidad ademas de read con un pipe
*/

int main (int argc, char *argv[]){
	
	 
	int fd;
	GHeader* cabeza;
	
	
	if (argc != 2) {
		perror("error en las cantidad de argumentos");
		return EXIT_FAILURE;
	} else {
		
		//si pasaron la cantidad de parametro correctos abrimos el archivo
		 fd = open(argv[1], O_RDONLY);
           if (fd == -1) printf("error al abrir al archivo binario");
	}

//prototipo void *mmap(void *addr, size_t length, int prot, int flags,int fd, off_t offset);
		    cabeza = (GHeader*) mmap(NULL, BLOQUE, PROT_READ, MAP_SHARED,fd, NULL);
	


printf("que tengo en la cabeza? %s \n",cabeza->grasa);






	return 0;
}
