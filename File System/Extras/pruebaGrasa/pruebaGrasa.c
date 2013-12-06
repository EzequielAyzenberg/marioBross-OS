#include "grasa.h"
//#include <string.h> //para strcat
#include <sys/mman.h> //biblioteca para usar mmap y sus derivadas
#include <commons/bitarray.c> //para las funciones que manejan el BitMap
#include <commons/collections/list.c> //para devolver listas en queHayAca
#include <commons/string.h>//para usar split en how

#define BLOQUE 4096
#define DISCO 10485760
#define FAIL -1
#define BLOCK_INDIREC_SIZE 1024
#define MAXNODO 1024



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
ptrGBloque bloqueDondeEstaElByte(GFile* nodo,int byte);
char* lastNameFromPath(char* path);
//char** dameLosNombresDelPath(char* );

int main (int argc, char *argv[]){
	
	 
	int fd;
    
	int bloque,i;
	extern uint8_t* ptr_mmap;
	extern GHeader* ptr_header;
	extern GFile* ptr_nodo;
	extern t_bitarray* bitMap;
	ptrGBloque ptrb;
	char** nombre;
	char* path;
	
	//t_list* lista = list_create();
	
	
	

	//apertura del Disco
	if (argc != 2) {
		perror("error en las cantidad de argumentos");
		return EXIT_FAILURE;
	}
	else {//si pasaron la cantidad de parametro correctos abrimos el archivo
		 fd = open(argv[1], O_RDWR);
           if (fd == -1) printf("error al abrir al archivo binario");
    }
    ptr_mmap = mmap(NULL, DISCO, PROT_READ|PROT_WRITE, MAP_SHARED,fd, NULL);
    ptr_header = (GHeader*) dir_bloque(0);
    ptr_nodo = (GFile*) dir_bloque(ptr_header->size_bitmap);
    bitMap = bitarray_create((char*)dir_bloque(1), 320);
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
	extern t_bitarray* bitMap;
	int numpadre;
	const char* path = "/dir1/prueba.txt";
	char* newPath;
	char copyNewPath[72];
	char* nombre;
	char* buffer;
	int res;
		
	//path = string_new();
	//nodo = (GFile*) mmap(NULL, DISCO, PROT_READ, MAP_SHARED,archDisk, BLOQUE);
	//strcpy(path,"/dir1/hola.txt");
	//strcpy(path,"/dir1/secret");
	//strcpy(path,"/otro_dir");
	//strcpy(path,"/ar");
	
	//printf("cual es el path %s\n",newPath);
	
	//printf("total de / \n%d",strspn(path,"/"));
	
	
	//printf("desaparecio / ? %s\n",*(string_split("/dir1","/")));
	
/*	
	t_list* archivos;
	archivos = list_create();
	numpadre=nodoByPath(path,ptr_nodo);
	
	if (numpadre==FAIL) printf("archivo no existe man\n");
	
	else{
	
	queHayAca(numpadre,ptr_nodo,archivos);
		

	for (i=0; i < list_size(archivos); i++){
	nombre =(char*)list_get(archivos,i);
	printf("que hay en path%s: %s\n",path,nombre);
	}

list_destroy(archivos);
	
	i=nodoByPath(path,ptr_nodo);
	
	printf("%d num",i);
	
	printf("estado: %d \n",ptr_nodo[i].state);
	printf("nombre: %s \n",ptr_nodo[i].fname);
	printf("padre %d \n",ptr_nodo[i].parent_dir_block);
	printf("tamanio %d \n",ptr_nodo[i].file_size);
	printf("fecha modificacion %d \n",(int)ptr_nodo[i].m_date);
	printf("fecha creacion %d \n",(int)ptr_nodo[i].c_date);
	printf("bloque de indireccion numero: %d \n", (int)ptr_nodo[i].blk_indirect[0]);
	printf("bloque de indireccion numero: %d \n", (int)ptr_nodo[i].blk_indirect[1]);
	
	//readGrid(buffer,(size_t)100000, (off_t)4097,ptr_nodo+i);
	
	
	
	}
	*/
	
	res=crearArchivo(path,ptr_nodo);
	howIsMyFather(path,&newPath);
	printf("que hay en newPath: %s\n",newPath);
	//strcpy(copyNewPath,newPath);
	//printf("que hay en CopyNewPath: %s\n",copyNewPath);
	//crearDirectorio(path,ptr_nodo);
	numpadre=nodoByPath(path,ptr_nodo);
	if(res==FAIL)puts("como existe le voy a truncar");
	
	truncale(path,100,ptr_nodo,bitMap);
	//PARA REINICIAR
	//ptr_nodo[numpadre].file_size=0;
	//ptr_nodo[numpadre].blk_indirect[1]=0;	
	//ptr_nodo[numpadre].blk_indirect[2]=0;	
	//ptr_nodo[numpadre].blk_indirect[3]=0;
	printf("nuevo size: %d\n",ptr_nodo[numpadre].file_size);
	
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
	int numNodo;
	int acierto;
	acierto=0;
	int numHijo;
	numHijo=0;
	int encontrado;
	
	int numPadre;
	
	
	
	nombreHijo = string_split((char*)path,"/");
	
	
	numPadre=0;
	
	//falta cambiar equals
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
   ptr_datos = (uint8_t*)dir_bloque(bloqueDondeEstaElByte(nodo,(int) bytePedido));
   bool esElPrimero = bytesLeidos == 0;  
   int offsetDentroDelBloque = esElPrimero ? offset % BLOQUE : 0;
   int bytesRestantesDentroDelBloque = BLOQUE - offsetDentroDelBloque;

   int bytesParaCopiar = size - bytesLeidos;
   if (bytesParaCopiar > bytesRestantesDentroDelBloque)
      bytesParaCopiar = bytesRestantesDentroDelBloque;
	
	buf=(char*)malloc(size);
   memcpy(buf + bytesLeidos, ptr_datos + offsetDentroDelBloque, bytesParaCopiar);
   bytesLeidos += bytesParaCopiar;
   bytePedido += bytesParaCopiar;
   
	}

	
}

//probar
int blkInd_by_cantBlk(int bloquesDatos)
{
	return bloquesDatos/1024;  	 //si el bloque de datos es mayor a 1024 ya pertenece al indirecto de la pocision 1

}

//modificada probar
ptrGBloque* arrayIndex(GFile* nodo, int byte){
	
	
	int numeroBloqueDato;
	int dirBloqueArray;
	int offsetDelArray;
	ptrGBloque* ptrBloque;
	
	printf("estoy en la funcion arrayIndex\n");
	printf("el nodo que llego es: %s\n",nodo->fname);	
	numeroBloqueDato = byte/BLOQUE; //desde cual bloque de datos voy a leer
	offsetDelArray = numeroBloqueDato%1024;
	dirBloqueArray=nodo->blk_indirect[blkInd_by_cantBlk(numeroBloqueDato)]; //tengo la direcion del bloque de array con puntero a bloques de datos
	printf("tengo la direcion del bloque de array con puntero a bloques de datos: %d \n",dirBloqueArray);
	ptrBloque =(ptrGBloque*) dir_bloque(dirBloqueArray); //ahora estoy apuntando a la primera posicion del bloque de array con punteros a bloque de datos
	printf("ahora estoy apuntando a la primera posicion del bloque de array con punteros a bloque de datos: %d \n",*ptrBloque);
	ptrBloque=ptrBloque+offsetDelArray;         //ahora estoy apundo a la direccion del bloque que quiero leer
	printf("ahora estoy apundo a la direccion del bloque que quiero leer: %d \n",*ptrBloque);
	
	return ptrBloque;
}


//esto me esta devolviendo el puntero al bloque numero de la primer posicion. 
ptrGBloque bloqueDondeEstaElByte(GFile* nodo,int byte){
	
	ptrGBloque* aux;
	printf("estoy en la funcion bloqueDondeEstaElByte\n");
	printf("el no que llego es: %s\n",nodo->fname);
	printf("el numero de bloque que voy a poner en dir bloque es: %d\n",(aux=arrayIndex(nodo,byte),*aux));
	return *aux;   //ahora estoy AL FIN estoy apuntando al la primer posicion del bloque de datos que tengo que leer

}


int crearDirectorio(const char* path,GFile* inodo){
	int numNodo;
	int nodoPadre;
	int nodoLibre;
	int i;
	char* pathPadre;
	nodoLibre=0;
	i=0;
	
	printf("la ruta que llego es: %s\n",path);
	
	numNodo=nodoByPath(path,inodo);
	
	printf("el resultado de nodobypath es: %d\n",numNodo);
	if (numNodo!=FAIL) puts("entonces el directorio no existe\n");
	if (numNodo!=FAIL) return -EEXIST;
	
	
	else {
	puts("pase a crear\n");
	howIsMyFather((char*)path,&pathPadre);
	printf("la ruta padre es: %s\n",pathPadre);
	nodoPadre=nodoByPath(pathPadre,inodo);
	printf("el resultado de nodobypath es: %d\n",nodoPadre);
	while((inodo[nodoLibre].state!=0)&&(nodoLibre<1024))nodoLibre++;
	
	
	printf("el primer nodo libre es: %d\n",nodoLibre);
	printf("El que quiero agragar es el last name: %s\n",lastNameFromPath((char*)path));
	//grabo
	inodo[nodoLibre].state=2;
	strcpy(inodo[nodoLibre].fname,lastNameFromPath((char*)path));  
	inodo[nodoLibre].parent_dir_block=nodoPadre;
	inodo[nodoLibre].m_date=111111;
	inodo[nodoLibre].c_date=111111;
	inodo[nodoLibre].file_size=0;
	
	return 0;
	}
	
		
}

int howIsMyFather(char* src, char** dest){
	
	
	char* lastName;
	int tamRuta;
	int i=0;
	
	
	
	puts("entre a how is my father\n");
	printf("la ruta que entro es: %s\n",src);	
	printf("El que quiero agragar es el last name: %s\n",lastNameFromPath(src));   
	tamRuta= strlen(src) - strlen(lastNameFromPath(src));
	printf("tamaño de la ruta: %d\n",tamRuta);
	*dest=string_duplicate(string_substring_until(src,strlen(src) - (strlen(lastNameFromPath(src))+1)));
	printf("la ruta donde lo agrego es: %s\n",*dest);
	
	
	
	
	
	
	return 0;
	
}

char* lastNameFromPath(char* path){
	char** subDirectorios;
	subDirectorios = string_split(path,"/");
	return subDirectorios[strlenTwo(subDirectorios)-1];
}


int strlenTwo(char**str){
	int i=0;
		while(str[i])i++;
	return i;
}


int crearArchivo(const char* path,GFile* inodo){
	int numNodo;
	int nodoPadre;
	int nodoLibre;
	int i;
	char* pathPadre;
	nodoLibre=0;
	i=0;
	
	printf("la ruta que llego es: %s\n",path);
	
	numNodo=nodoByPath(path,inodo);
	
	printf("el resultado de nodobypath es: %d\n",numNodo);
	if (numNodo!=FAIL) 
	{
		puts("entonces el archivo ya existe\n");
		inodo[nodoLibre].m_date=1381272974;
		return 0;
	}
	
	else {
	puts("pase a crear\n");
	howIsMyFather((char*)path,&pathPadre);
	printf("la ruta padre es: %s\n",pathPadre);
	nodoPadre=nodoByPath(pathPadre,inodo);
	printf("el resultado de nodobypath es: %d\n",nodoPadre);
	while((inodo[nodoLibre].state!=0)&&(nodoLibre<1024))nodoLibre++;
	
	
	printf("el primer nodo libre es: %d\n",nodoLibre);
	printf("El que quiero agragar es el last name: %s\n",lastNameFromPath((char*)path));
	//grabo
	inodo[nodoLibre].state=1;
	strcpy(inodo[nodoLibre].fname,lastNameFromPath((char*)path));  
	inodo[nodoLibre].parent_dir_block=nodoPadre;
	inodo[nodoLibre].m_date=(uint64_t)1381272974;
	inodo[nodoLibre].c_date=(uint64_t)1381272974;
	inodo[nodoLibre].file_size=0;
	
	return 0;
	}
	
	
}

int bloquesBySize(int size)
{
	
	return (size%BLOQUE==0) ?  size/BLOQUE : (size/BLOQUE)+1;
	
}

/*mmmmmmmmmmmm esta queda?
ptrGBloque* ultimoBloqueReferenciado(GFile* nodo)
{
	printf("estoy en la funcion bloqueDondeEstaElByte\n");
	printf("el no que llego es: %s",nodo->fname);
	return (ptrGBloque*)bloqueDondeEstaElByte(nodo,nodo->file_size);
}
*/

int asignarBloque(t_bitarray* bMap)
{	int i=0;
	while(bitarray_test_bit(bMap,i))i++;
	printf("que bloque le di: %d\n",i);  
	bitarray_set_bit(bMap,i);
	return i;
}

/* 
 * @Nombre: asignarIndirecto
 * @Precondicion: nodo donde va a ser cargado el array de 1024 bloque de datos
 * @posCondicion: puntero de tipo ptrGBloque a la primer posicion de array de datos
 */

ptrGBloque* asignarIndirecto(t_bitarray* bMap,GFile* archivo,int bloqueDatos)
{
	return (ptrGBloque*) (archivo->blk_indirect[blkInd_by_cantBlk(bloqueDatos)]=asignarBloque(bMap),dir_bloque(archivo->blk_indirect[blkInd_by_cantBlk(bloqueDatos)]));
}

/*
 * @Nombre: difEntreIndirectos
 * @Precondicion: cantidad de bloques actuales, cantidad de bloques nuevos
 * @posCondicion: devuelve un numero positivo si hay diferencia entre bloques indirectos y ese numero es la diferencia. sino devuelve 0 
 */


int difEntreIndirectos(int bActual,int bNuevos)
{
	return blkInd_by_cantBlk(bActual)<blkInd_by_cantBlk(bNuevos) ? blkInd_by_cantBlk(bNuevos)-blkInd_by_cantBlk(bActual) : blkInd_by_cantBlk(bActual)-blkInd_by_cantBlk(bNuevos);
}

int liberarBloque(int byte,t_bitarray* bMap,GFile* nodo)
{
	ptrGBloque* pgBloque=arrayIndex(nodo,byte);
	printf("que bloque voy a liberar: %d\n", *pgBloque);
	bitarray_clean_bit(bMap,*pgBloque);
	printf("la disponibilidad del bloque %d es: %s\n",bloqueDondeEstaElByte(nodo,byte), bitarray_test_bit(bMap,bloqueDondeEstaElByte(nodo,byte))? puts("ocupado"):puts("desocupado"));
return 0;
}

/* @Nombre: liberarIndirectos
 * @Precondicion: el bitmap, los nodos y la diferencia entre bloques indirectos
 * @posCondicion: devuelve 0 si seteo a 0 los bloques correspondientes 
 */

int liberarIndirecos(t_bitarray* bMap,GFile* nodo,int dif)
{
	int ind = blkInd_by_cantBlk(bloquesBySize(nodo->file_size));
	int i;
	while(dif)
	{
		printf("que bloque voy a liberar: %d\n", nodo->blk_indirect[ind]);
		bitarray_clean_bit(bMap,nodo->blk_indirect[ind]);
		printf("la disponibilidad del bloque %d es: %d\n",nodo->blk_indirect[ind], bitarray_test_bit(bMap,nodo->blk_indirect[ind])? puts("ocupado"):puts("desocupado"));
		ind--;
		dif--;
	}  
return 0;
}

/* @Nombre: iniciarNodo
 * @Precondicion: el bitmap, un nodo con size 0
 * @posCondicion: devuelve el puntero a la primer posicion de bloque de array de datos. SIN un bloque asignado.  
 */
ptrGBloque* iniciarNodo(t_bitarray* bMap,GFile* nodo)
{
	return (nodo->blk_indirect[0]=asignarBloque(bMap),(ptrGBloque*)dir_bloque(nodo->blk_indirect[0]));
}

int truncale(const char* path,off_t offset,GFile* nodo,t_bitarray* bMap)
{
	int numNodo= nodoByPath(path,nodo);
	int byteRef;
	ptrGBloque* pGBloque;
	
	
	//Si el archivo anteriormente era más grande que este tamaño, se pierden los datos extra. 
	
	//Si el archivo anteriormente era más corto, SI se extendió dentro del bloque actualmente asignado me quedo piola 
	//											 SI se extendió al proximo bloque me fijo cuantos bloques SI es el proximo bloque estas dentro de los 1024
	//		
	//																								  SI esta en proximo inodo, pido otro bloque de 1024.				 
	printf("el archivo que vamos a truncar es: %s\n",nodo[numNodo].fname);
	int bloquesDatosActuales = bloquesBySize(nodo[numNodo].file_size);  //HECHO
	printf("los  bloquesDatosActuales son: %d\n",bloquesDatosActuales);
	int bloquesDatosOffset = bloquesBySize(offset);
	printf("los bloquesDatosOffset son: %d\n",bloquesDatosOffset);
	printf("El file size es: %d\n",nodo[numNodo].file_size);
	
	//if (nodo[numNodo].file_size==0 && offset<BLOQUE) {pGBloque=iniciarNodo(bMap,nodo+numNodo),*pGBloque=asignarBloque(bMap);} 
	
	if (bloquesDatosActuales < bloquesDatosOffset)   //se agranda informo el nuevo size del archivo.
	{
	  puts("entoy agrandando\n");
	 if(nodo[numNodo].file_size) 
	 {
		 pGBloque = arrayIndex(nodo+numNodo,nodo[numNodo].file_size);
		 printf("el pGBloque apuanta antes del ++ a: %p\n",pGBloque);
		 printf("el ultimo pGBloque de este archivo es: %d\n",*pGBloque);
		 pGBloque++;
		 printf("el pGBloque apuanta despues del ++ a: %p\n",pGBloque);
	 }	
	//else pGBloque=iniciarNodo(bMap,nodo+numNodo);  //si es 0 y offset es mayor a un bloque pasa por aca
	    //pongo esto afuera y soluciono? si lo saco entra a while y el if de verdadero
	 
	  	while(bloquesDatosActuales < bloquesDatosOffset)
	  	{
			//que pasa si entra con 0, si tiene uno cargado LA CAGA.
			 printf("ultimo indirecto antes de chequear: %d\n",nodo[numNodo].blk_indirect[blkInd_by_cantBlk(bloquesDatosActuales)]);    
			 if(!bloquesDatosActuales%BLOCK_INDIREC_SIZE) {pGBloque = asignarIndirecto(bMap,nodo+numNodo,bloquesDatosActuales); puts("entre al if de asignar otro bloque indirecto");}  
			 printf("ultimo indirecto despues de chequear: %d\n",nodo[numNodo].blk_indirect[blkInd_by_cantBlk(bloquesDatosActuales)]);
			 *pGBloque=asignarBloque(bMap);   
			 printf("el resultado de pGBloque es: %d\n",*pGBloque);
			 bloquesDatosActuales++;
			 //printf("anterior indirecto despues de chequear: %d",nodo->blk_indirect[blkInd_by_cantBlk(bloquesDatosActuales));
			 pGBloque++;
			 printf("el pGBloque apuanta despues del ++ a: %p\n",pGBloque);
		}
	}
	
	else{
			puts("entre achicar\n");
			if (bloquesDatosActuales > bloquesDatosOffset) // se achica  informo el nuevo size del achivo, con eso deberia ser suficiente para saber como leer
			{
				puts("estoy achicando\n");
				int difBloquesIndirectos = difEntreIndirectos(bloquesDatosActuales,bloquesDatosOffset); //HECHO
				printf("la diferencia entre indirectos es: %d\n", difBloqueIndirectos);
				byteRef = offset+BLOQUE;                                            //(offset%BLOQUE==0) ?  offset+1 : offset + (BLOQUE-offset%BLOQUE) + 1;  
				while(bloquesDatosActuales > bloquesDatosOffset)                
				{
				 puts("estoy en while de achicar");	
				 liberarBloque(byteRef,bMap,nodo+numNodo);   //HECHO
				 byteRef=+BLOQUE;
				 bloquesDatosActuales--;
				}
				if (difBloquesIndirectos) liberarIndirecos(bMap,nodo+numNodo,difBloquesIndirectos);   //HECHO
			}
				
	}
	
	
	nodo[numNodo].file_size=(uint32_t)offset;


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
	
	
/*
int cargarBuffer(char *buf, size_t size, off_t offset,GFile* inodo){
	
	int hastaCualBlk_indirectLeo;
	int desdeCualBlk_indirectLeo;
	int hastaCualBlk_directLeo;
	int deCualBlk_directLeo;
	int cuantosBloquesLeo;
	int offsetDelPrimero;
	int cuantoLeoDelUltimo;
	int dirBloqueArray;
	int offsetBuff;
	ptrGBloque* ptrBloque;
	
	uint8_t* ptr_datos;
	
	printf("soy el nodo: %s dentro de cargar buffer \n",inodo[0].fname);
	
	deCualBlk_directLeo = offset/BLOQUE; //desde cual bloque de datos voy a leer
		
	hastaCualBlk_directLeo = (size+offset)/BLOQUE; //hasta cual bloque de datos voy a leer
	//if(((size+offset)%BLOQUE)>0) hastaCualBlk_directLeo += 1;
	offsetDelPrimero = offset%BLOQUE;
	cuantoLeoDelUltimo = (size + offset)%BLOQUE;
	desdeCualBlk_indirectLeo = deCualBlk_directLeo/1024;
	hastaCualBlk_indirectLeo = hastaCualBlk_directLeo/1024; 
	
	if(deCualBlk_directLeo==hastaCualBlk_directLeo)cuantosBlk_directLeo=CASO1;
	if(deCualBlk_directLeo+1==hastaCualBlk_directLeo)cuantosBlk_directLeo=CASO2;
	if(deCualBlk_directLeo+1<hastaCualBlk_directLeo)cuantosBlk_directLeo=CASO3;	
	
	printf("soy el caso: %d\n",cuantosBlk_directLeo);
	 
	
	printf("cuantosBlk_indirectLeo: %d \n",hastaCualBlk_indirectLeo);
	printf("deCualBlk_indirectEmpiezo: %d \n",desdeCualBlk_indirectLeo);
	printf("hastaCualBlk_directLeo: %d \n",hastaCualBlk_directLeo);
	printf("deCualBlk_directEmpiezo: %d \n",deCualBlk_directLeo);
	printf("cuanto leo del ultimo: %d \n",cuantoLeoDelUltimo);
	printf("offset del primero: %d \n",offsetDelPrimero);
	
	buf=(char*)malloc(size);
	
	//operatoria para apuntar al primer bloque que tengo que leer
	
	dirBloqueArray=inodo->blk_indirect[desdeCualBlk_indirectLeo]; //tengo la direcion del bloque de array con puntero a bloques de datos
	printf("tengo la direcion del bloque de array con puntero a bloques de datos: %d \n",dirBloqueArray);
	ptrBloque =(ptrGBloque*) dir_bloque(dirBloqueArray); //ahora estoy apuntando a la primera posicion del bloque de array con punteros a bloque de datos
	printf("ahora estoy apuntando a la primera posicion del bloque de array con punteros a bloque de datos: %d \n",*ptrBloque);
	ptrBloque=ptrBloque+deCualBlk_directLeo;         //ahora estoy apundo a la direccion del bloque que quiero leer
	printf("ahora estoy apundo a la direccion del bloque que quiero leer: %d \n",*ptrBloque);
	ptr_datos = (uint8_t*)dir_bloque(*ptrBloque);   //ahora estoy AL FIN estoy apuntando al la primer posicion del bloque de datos que tengo que leer
	ptrBloque+=1;	                                //ahora apuntado a la proxima direccion del bloque de datos que tengo que leer
	printf("ahora apuntado a la proxima direccion del bloque de datos que tengo que leer: %d \n",*ptrBloque);
	
	
	switch(cuantoBlk_directLeo)
	{
	
	case CASO1:
	puts("entre en el caso 1");
	memcpy(buf,ptr_datos+offsetDelPrimero,size);
	break;
	
	
	case CASO2:
	puts("entre en el caso 2");
	puts("leo el primero");
	memcpy(buf,ptr_datos+offsetDelPrimero,BLOQUE-offsetDelPrimero);   //copio los primero datos, aquellos cuyos podrian tener offset De Bloque
	offsetBuff=BLOQUE-offsetDelPrimero;     //actulizo el offset del buff para que no se sobresquiban los datos
	deCualBlk_directLeo+=1
	puts("leo el segundo");
	
	
	
	}
	
	
	
	//primer lectura, el offset
	memcpy(buf,ptr_datos+offsetDelPrimero,BLOQUE-offsetDelPrimero);   //copio los primero datos, aquellos cuyos podrian tener offset De Bloque
	offsetBuff=BLOQUE-offsetDelPrimero;     //actulizo el offset del buff para que no se sobresquiban los datos
	deCualBlk_directLeo+=1;                         //actualizo desde donde debo leer
	
	//esto es por si hay intermedios
	do{
			puts("entre en los intermedios");
		
			while((deCualBlk_directLeo<hastaCualBlk_directLeo)||((deCualBlk_directLeo%1024)==0)){  //sin desde es menor que hasta entro y si no estoy apuntando al ultimo bloque 
			deCualBlk_directLeo+=1;                         //actualizo desde donde debo leer
			ptr_datos = (uint8_t*)dir_bloque(*ptrBloque);   //ahora estoy apuntando a la primer posicion de memoria del bloque de datos que quiero cargar en el buffer
			memcpy(buf+offsetBuff,ptr_datos,BLOQUE); 		//copio los datos
			offsetBuff+=BLOQUE;  						    //actulizo el offset del Bloque
			ptrBloque+=1;                                   //ahora estoy apundo a la direccion del bloque que quiero leer
			puts("entre al while de los intermedios");
			}
		desdeCualBlk_indirectLeo+=1;						//actulizo desde donde leo los directos
		//aca hay lio
		if(inodo->blk_indirect[desdeCualBlk_indirectLeo]) //si el proximo indirecto es no es 0
		{
		dirBloqueArray=inodo->blk_indirect[desdeCualBlk_indirectLeo];
	    ptrBloque =(ptrGBloque*) dir_bloque(dirBloqueArray);
	    }
		
	}while(desdeCualBlk_indirectLeo<hastaCualBlk_indirectLeo);
	
	//Esto es para guardar en el buff lo que queda del ultimo bloque, si es que queda
	if(cuantoLeoDelUltimo) 
	{
		puts("tengo resto");
	ptr_datos = (uint8_t*)dir_bloque(*ptrBloque);   //ahora estoy apuntando a la primer posicion de memoria del bloque de datos que quiero cargar en el buffer
	memcpy(buf+offsetBuff,ptr_datos,cuantoLeoDelUltimo); 		//copio los datos que faltan del ultimo bloque
	}
	
	return 0;
}
*/
