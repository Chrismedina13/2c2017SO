/*
 * FileSystem.h
 *
 *  Created on: 7/9/2017
 *      Author: utnso
 */

#ifndef HEADERS_FILESYSTEM_H_
#define HEADERS_FILESYSTEM_H_
#define MAX 1000
#include "configuracion.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "socketsFileSystem.h"
#include "logFileSystem.h"
#include "pthread.h"
#include "comunicacionConDN.h"
#include "comunicacionConYama.h"
#include <commons/collections/list.h>
#include "SO_lib/estructuras.h"
#include "commons/bitarray.h"
#include "commons/string.h"


#define ARCHIVO_CONFIGURACION "/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/FileSystem/filesystem.conf"


//tabla de directorios

struct tabla_directorios {
  int index;
  char nombre[255];
  int padre;
}tabla_directorios; // registros dentro de tabla_de_directorios


struct tabla_directorios tabla_de_directoriosAux[100], tabla_de_directorios[100]; //declaracion global de la estructura

//tabla de archivos

typedef struct tabla_archivos {
	int tamanio; //en bytes
	char tipo;
	int directorio; //index del directorio
	UbicacionBloquesArchivo* bloques;
}tabla_archivos;

//tabla_archivos* archivosPtr;

//tabla de nodos

typedef struct bloques_nodos{
	int bloqueslibres;
	int bloquestotales;
}bloques_nodos;

struct tabla_nodos {
	int tamanio; //cantidad de bloques TOTAL ENTRE TODOS LOS NODOS
	int bloqueslibres; //cantidad de bloques libres entre todos los nodos
	t_list* listaNodos; //nodo1, nodo2, nodo3
	bloques_nodos* nodosCapacidad; //total por nodo, y libre
}tabla_nodos;

struct tabla_nodos tabla_de_nodos;

struct bloques_nodos bloques_por_nodo;


t_list* bloquesNodos(); // en cada pos un registro bloques_nodos


int bitmap[20];
t_list* listabitmap_nodo(); //  un bit map por cada nodo









#endif /* HEADERS_FILESYSTEM_H_ */

