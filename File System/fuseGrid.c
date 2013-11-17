#include <stddef.h>
#include <stdlib.h>
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include "grasa.c"





/*
 * Esta es una estructura auxiliar utilizada para almacenar parametros
 * que nosotros le pasemos por linea de comando a la funcion principal
 * de FUSE
 */
struct t_runtime_options {
	char* welcome_msg;
} runtime_options;

GFile* ptr_nodo;
GHeader* ptr_header;
uint8_t* ptr_mmap;
/*
 * Esta Macro sirve para definir nuestros propios parametros que queremos que
 * FUSE interprete. Esta va a ser utilizada mas abajo para completar el campos
 * welcome_msg de la variable runtime_options
 */
#define CUSTOM_FUSE_OPT_KEY(t, p, v) { t, offsetof(struct t_runtime_options, p), v }




/*
 * @DESC
 *  Esta función va a ser llamada cuando a la biblioteca de FUSE le llege un pedido
 * para obtener la metadata de un archivo/directorio. Esto puede ser tamaño, tipo,
 * permisos, dueño, etc ...
 *
 * @PARAMETROS
 * 		path - El path es relativo al punto de montaje y es la forma mediante la cual debemos
 * 		       encontrar el archivo o directorio que nos solicitan
 * 		stbuf - Esta esta estructura es la que debemos completar
 *
 * 	@RETURN
 * 		O archivo/directorio fue encontrado. -ENOENT archivo/directorio no encontrado
 */
static int theGrid_getattr(const char *path, struct stat *stbuf) {
	
	

	memset(stbuf, 0, sizeof(struct stat));

	//Si path es igual a "/" nos estan pidiendo los atributos del punto de montaje
	
	extern GFile* ptr_nodo;
	int numNodo;
	

    
	 numNodo=nodoByPath(path,ptr_nodo);
	
	if (strcmp(path, "/") == 0) {
			stbuf->st_mode = S_IFDIR | 0755;
			stbuf->st_nlink = 2;
			return 0;
			
		}
    

	if(numNodo!=FAIL){
		
						if(ptr_nodo[numNodo].state==2){
							stbuf->st_mode = S_IFDIR | 0755;
							stbuf->st_nlink = 1;
						}
						else{
							stbuf->st_mode = S_IFREG | 0444;
							stbuf->st_nlink = 1;
							stbuf->st_size = ptr_nodo[numNodo].file_size;
						}
	return 0;
	}
	
	else return -ENOENT;
	
	
	

}





/*
 * @DESC
 *  Esta función va a ser llamada cuando a la biblioteca de FUSE le llege un pedido
 * para obtener la lista de archivos o directorios que se encuentra dentro de un directorio
 *
 * @PARAMETROS
 * 		path - El path es relativo al punto de montaje y es la forma mediante la cual debemos
 * 		       encontrar el archivo o directorio que nos solicitan
 * 		buf - Este es un buffer donde se colocaran los nombres de los archivos y directorios
 * 		      que esten dentro del directorio indicado por el path
 * 		filler - Este es un puntero a una función, la cual sabe como guardar una cadena dentro
 * 		         del campo buf
 *
 * 	@RETURN
 * 		O directorio fue encontrado. -ENOENT directorio no encontrado
 */
static int theGrid_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
	(void) offset;
	(void) fi;
	extern GFile* ptr_nodo;
	char* nombre;
	int numNodo;
	int i;

	t_list* archivos;
	archivos = list_create();
	
	numNodo=nodoByPath(path,ptr_nodo);
	
	if (numNodo==FAIL) return -ENOENT;
	
	else {
	queHayAca(numNodo,ptr_nodo,archivos);
	
	
	

	// "." y ".." son entradas validas, la primera es una referencia al directorio donde estamos parados
	// y la segunda indica el directorio padre
	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);

	for (i=0; i < list_size(archivos); i++){
	nombre =(char*)list_get(archivos,i);
	filler(buf, nombre, NULL, 0);
	}

list_clean(archivos);
list_destroy(archivos);

	return 0;
	}
}

/*
 * @DESC
 *  Esta función va a ser llamada cuando a la biblioteca de FUSE le llege un pedido
 * para tratar de abrir un archivo
 *
 * @PARAMETROS
 * 		path - El path es relativo al punto de montaje y es la forma mediante la cual debemos
 * 		       encontrar el archivo o directorio que nos solicitan
 * 		fi - es una estructura que contiene la metadata del archivo indicado en el path
 *
 * 	@RETURN
 * 		O archivo fue encontrado. -EACCES archivo no es accesible
 */
static int theGrid_open(const char *path, struct fuse_file_info *fi) {
	int res;
	res=0;
	
	
	res = nodoByPath(path,ptr_nodo);
	if ((fi->flags & 3) != O_RDONLY)
		return -EACCES;

	if (res == FAIL) return -ENOENT;
	return 0;
}

/*
 * @DESC
 *  Esta función va a ser llamada cuando a la biblioteca de FUSE le llege un pedido
 * para obtener el contenido de un archivo
 *
 * @PARAMETROS
 * 		path - El path es relativo al punto de montaje y es la forma mediante la cual debemos
 * 		       encontrar el archivo o directorio que nos solicitan
 * 		buf - Este es el buffer donde se va a guardar el contenido solicitado
 * 		size - Nos indica cuanto tenemos que leer
 * 		offset - A partir de que posicion del archivo tenemos que leer
 *
 * 	@RETURN
 * 		Si se usa el parametro direct_io los valores de retorno son 0 si  elarchivo fue encontrado
 * 		o -ENOENT si ocurrio un error. Si el parametro direct_io no esta presente se retorna
 * 		la cantidad de bytes leidos o -ENOENT si ocurrio un error. ( Este comportamiento es igual
 * 		para la funcion write )
 */
static int theGrid_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
	size_t len;
	(void) fi;
	int numNodo;
	extern GFile* ptr_nodo;
	numNodo=nodoByPath(path,ptr_nodo);
	len = ptr_nodo[numNodo].file_size;
	
	if (numNodo==FAIL) return -ENOENT;

	
	if (offset<len) { 
		if (offset + size > len) size = len - offset;
		
	     readGrid(buf,size,offset,ptr_nodo+numNodo);
	    
	}
	else size = 0;
	
		
	return size;
}

/*
static int32_t pfs_mkdir(const char * path,mode_t mode)
{   int res;
	
	res = crearDirectorio(path);
	return res;
}
*/

/*
 * Esta es la estructura principal de FUSE con la cual nosotros le decimos a
 * biblioteca que funciones tiene que invocar segun que se le pida a FUSE.
 * Como se observa la estructura contiene punteros a funciones.
 */

static struct fuse_operations theGrid_oper = {
		.getattr = theGrid_getattr,
		.readdir = theGrid_readdir,
		.open = theGrid_open,
		.read = theGrid_read,
		//.mkdir = theGrid_mkdir, 
};


/** keys for FUSE_OPT_ options */
enum {
	KEY_VERSION,
	KEY_HELP,
};


/*
 * Esta estructura es utilizada para decirle a la biblioteca de FUSE que
 * parametro puede recibir y donde tiene que guardar el valor de estos
 */
static struct fuse_opt fuse_options[] = {
		// Este es un parametro definido por nosotros
		CUSTOM_FUSE_OPT_KEY("--welcome_msg", welcome_msg, 0),
		//CUSTOM_FUSE_OPT_KEY("--disk.bin", welcome_msg, 0),
		

		// Estos son parametros por defecto que ya tiene FUSE
		FUSE_OPT_KEY("-V", KEY_VERSION),
		FUSE_OPT_KEY("--version", KEY_VERSION),
		FUSE_OPT_KEY("-h", KEY_HELP),
		FUSE_OPT_KEY("--help", KEY_HELP),
		FUSE_OPT_END,
};

// Dentro de los argumentos que recibe nuestro programa obligatoriamente
// debe estar el path al directorio donde vamos a montar nuestro FS
int main(int argc, char *argv[]) { //./fuse  mnt -f -disk disk.bin
	struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
    int fd;
	// Limpio la estructura que va a contener los parametros
	memset(&runtime_options, 0, sizeof(struct t_runtime_options));

	// Esta funcion de FUSE lee los parametros recibidos y los intepreta
	if (fuse_opt_parse(&args, &runtime_options, fuse_options, NULL) == -1){
		/** error parsing options */
		perror("Invalid arguments!");
		return EXIT_FAILURE;
	}

	// Si se paso el parametro --welcome-msg
	// el campo welcome_msg deberia tener el
	// valor pasado
	if( runtime_options.welcome_msg != NULL ){
		printf("%s\n", runtime_options.welcome_msg);
		//fd = open(runtime_options.welcome_msg, O_RDONLY);
	}
	
	//abro el archivo
	fd = open(argv[1], O_RDWR);
    if (fd == -1) printf("error al abrir al archivo binario");
    extern GFile* ptr_nodo;
    extern GHeader* ptr_header;
    extern uint8_t* ptr_mmap;
	ptr_mmap =(uint8_t*) mmap(NULL, DISCO, PROT_READ|PROT_WRITE, MAP_SHARED,fd,NULL);
	ptr_header = (GHeader*) dir_bloque(0);
    ptr_nodo = (GFile*) dir_bloque(1); 
	//nodos = dir_block(header->blk_bitmap + header->size_bitmap -1)
	
	// Esta es la funcion principal de FUSE, es la que se encarga
	// de realizar el montaje, comuniscarse con el kernel, delegar todo
	// en varios threads
	return fuse_main(args.argc -1, args.argv +1, &theGrid_oper, NULL);
}


void* dir_bloque(int n){
	extern uint8_t* ptr_mmap;
	return ptr_mmap + BLOQUE*n;	
}
