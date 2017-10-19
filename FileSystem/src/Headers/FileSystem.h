/*
 * FileSystem.h
 *
 *  Created on: 7/9/2017
 *      Author: utnso
 */

#ifndef HEADERS_FILESYSTEM_H_
#define HEADERS_FILESYSTEM_H_
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

struct tabla_archivos {
	int tamanio; //en bytes
	char tipo[255];
	int directorio; //index del directorio
	UbicacionBloquesArchivo* bloques;
}tabla_archivos;

struct tabla_archivos tabla_de_archivos;

//tabla de nodos

struct tabla_nodos {
	int tamanio; //cantidad de bloques TOTAL ENTRE TODOS LOS NODOS
	int bloqueslibres; //cantidad de bloques libres entre todos los nodos
	t_list* listaNodos; //nodo1, nodo2, nodo3
	bloques_nodos* bitMapNodos;
}tabla_nodos;

struct tabla_nodos tabla_de_nodos;

struct bloques_nodos{
	int bloqueslibres;
	int bloquestotales;
}bloques_nodos;

struct bloques_nodos bloque_de_nodos;

t_list* bloquesNodos(); // en cada pos un registro bloques_nodos

t_bitarray* bloques(); //bitmap de bloques por nodo.

void inicializarBitmap(t_bitarray bloques);
void imprimirResultado(t_bitarray bloques);






#endif /* HEADERS_FILESYSTEM_H_ */

