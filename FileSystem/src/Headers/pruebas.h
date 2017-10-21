/*
 * pruebas.h
 *
 *  Created on: 11/10/2017
 *      Author: utnso
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "commons/collections/list.h"
#include "commons/string.h"

typedef struct ArchivoEnMemoria{
	char* puntero;
	struct stat sb;
}ArchivoEnMemoria;

/*Funcion principal (TEXTO)*/
int pasarAMemoriaTxt(const char * rutaDelArchivo);

/*Funcion principal (BINARIOS)*/
int pasarAMemoriaBinario (const char* rutaArchivo);

/*Recibe como parametros la ruta del archivo y retorna:
 *  Una estructura formada por un puntero a la memoria donde se guardo el archivo (USO DE MMAP)
 * 	Un stat del archivo, que sirve para obtener el tamaño del archivo.
 */
ArchivoEnMemoria damePunteroYTamanio(const char* rutaArchivo);

/*Retorna la cantidad de bloques de 1KB se pueden hacer con un tamaño de archivo (FUNCION ESPECIAL PARA BINARIOS*/
int cantidadDeBloques(int tamanio);

/*Retorna una lista de RUTAS DE ARCHIVOS. Recibe como parametros un puntero a memoria y la cantidad de bloques que puede tener de 1 KB (BINARIO)*/
t_list* dividirArchivoBinario(char*puntero,int cantidadDeBloques);

/*Retorna una lista de RUTAS DE ARCHIVOS. Recibe como parametros un puntero a memoria (TEXTO)*/
t_list* dividirArchivoTxt(char* p);

//Funcion que imprime un archivo (CREADA PARA VER EL USO DE LAS LISTAS)
int imprimirArchivo(t_list* lista);

