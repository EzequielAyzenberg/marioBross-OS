#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <xcb/xcb.h>   //libreria chota para usar los int "especiales"

///////////////////DECLARACION DE CONSTANTE///////////////////
#define GFILEBYTABLE 1024
#define GFILEBYBLOCK 1
#define GFILENAMELENGTH 71
#define GHEADERBLOCKS 1
#define BLKINDIRECT 1000
						////mios
#define BLOQUE 4096
#define DISCO 10485760
#define MAXNODO 1023
#define FAIL -1
#define INICIO 0
#define BLOCK_INDIREC_SIZE 1024
#define LIBRE 0
#define OCUPADO 1
#define DIRECTORIO 2
#define CORRECCION 1    //esta constante esta pensada para hacer corresponder los inodos con los puntero 
						//asi el inodo 0 es el raiz pero accesiblemente es el dir1 del ejemplo, fue aclarado en un mail de la catedra

//////////////////FIN DE DECLARACION DE CONSTANTE/////////////

typedef uint32_t ptrGBloque;

typedef struct grasa_header_t { // un bloque
	unsigned char grasa[5];
	uint32_t version;
	uint32_t blk_bitmap;
	uint32_t size_bitmap; // en bloques
	unsigned char padding[4073];
} GHeader;

typedef struct grasa_file_t { // un cuarto de bloque (256 bytes)
	uint8_t state; // 0: borrado, 1: archivo, 2: directorio
	unsigned char fname[GFILENAMELENGTH];
	uint32_t parent_dir_block;
	uint32_t file_size;
	uint64_t c_date;
	uint64_t m_date;
	ptrGBloque blk_indirect[BLKINDIRECT];
} GFile;

int nodoByPath(const char* path,GFile* nodo);
int hijoDondeEstas(char* path,int padre,GFile* nodo);
int tablaDeNodos(int archDisk);

