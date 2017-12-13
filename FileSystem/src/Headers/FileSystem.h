/*
 *
 *  Created on: 7/9/2017
 *      Author: utnso
 */

#ifndef HEADERS_FILESYSTEM_H_
#define HEADERS_FILESYSTEM_H_
#define MAX 1000
#define MB 1024*1024

#include "configuracion.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "socketsFileSystem.h"
#include "logFileSystem.h"
#include "pthread.h"
#include "comunicacionConDN.h"
#include "comunicacionConYama.h"
#include <commons/collections/list.h>
#include "SO_lib/estructuras.h"
#include "commons/bitarray.h"
#include "commons/string.h"
#include "SO_lib/sem.h"
#include <dirent.h>
#include "commons/collections/list.h"
#include <semaphore.h>
#include <stdbool.h>



#define ARCHIVO_CONFIGURACION "/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/FileSystem/filesystem.conf"

//semaforos

Semaphore *SEMAFORODN;
Semaphore *SEMAFOROYAMA;
Semaphore *SEMAFOROESTADO;

//tabla de directorios

struct tabla_directorios {
  int index;
  char nombre[255];
  int padre;
}tabla_directorios; // registros dentro de tabla_de_directorios

//zona declaracion estructuras tabla de directorios

struct tabla_directorios tabla_de_directoriosAux[100], tabla_de_directorios[100]; //declaracion global de la estructura

//funciones de tabla de directorios

int cargarDirectorios();

int crearDirectorio(char* nombre, int padre);

int eliminarDirectorio(int index);

int cambiarNombreDirectorio(int index, char* nombre);

int moverDirectorio(int index, int padreNew);

int cantDir;

//tabla de archivos

typedef struct tabla_archivos {
	char* nombre;
	int tamanio; //en bytes
	char* tipo;
	int directorio; //index del directorio
	//UbicacionBloquesArchivo* bloques;
	t_list* bloques;//char *
	t_list* ubicaciones;//ubicacionBloquesArchivo
}tabla_archivos;

//zona declaracion estructuras tabla de archivos

struct tabla_archivos tabla_de_archivos[100];

//int maxArchivos = MAX;

extern tabla_archivos *archivosPtr;

//funciones de tabla de archivos

int crearRegistroArchivo(char* ruta, char* rutaLocal, int indiceArchivo);

int cambiarNombreArchivo(char* rutaLocal, char* nombreNew);

int moverArchivo(char* rutaLocal, char* newRutaLocal);

int mostrarArchivo(char* rutaLocal);

int eliminarArchivo(char* rutaLocal);

int tamanioArchivo(int fp);

int cantArchivos;

//tabla de nodos

typedef struct bloques_nodo{
	int idNodo;
	int fileDescriptor;
	int estado; //0 desconectado, 1 conectado
	int bloquesTotales;
	int bloquesLibres;
	int bitmap[160];
}bloques_nodo;

typedef struct tabla_nodos {
	int tamanio; //cantidad de bloques TOTAL ENTRE TODOS LOS NODOS
	int bloqueslibres; //cantidad de bloques libres entre todos los nodos
	t_list* listaNodos; //[1,2,3]
	t_list* listaCapacidadNodos; //bloques_nodo
}tabla_nodos;

struct tabla_nodos tabla_de_nodos;



typedef struct nodos_id_fd{
	int nodo_fd;
	int id_nodo;
}nodos_id_fd;

t_list* list_nodos_id_fd;

t_list* bloquesNodos(); // en cada pos un registro bloques_nodos

/*
typedef struct bitMap{
	int id_nodo;
	int bitmap[20];
}bitMap;
*/

t_list* listabitmap_nodo(); //  en esta lista pongo nodos

char* pathToType(char* path);
int nodoToFD(int idNodo);
char* pathToFile(char* path);

t_list* nombreToUbicaciones(char* nombre);

int cantNodos;
int cantNodosConectados;


#endif /* HEADERS_FILESYSTEM_H_ */

