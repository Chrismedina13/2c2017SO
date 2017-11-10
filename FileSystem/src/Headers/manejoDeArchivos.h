/*
 * manejoDeArchivos.h
 *
 *  Created on: 23/10/2017
 *      Author: utnso
 */

#ifndef HEADERS_MANEJODEARCHIVOS_H_
#define HEADERS_MANEJODEARCHIVOS_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "commons/collections/list.h"
#include "commons/string.h"


const int MB = 1024*1024;

/*
 * t_list* obtenerBloquesBinarios(const char * rutaDelArchivo);
 * RECIBE UNA RUTA DE UN ARCHIVO BINARIO A DIVIDIR,
 * RETORNA UNA LISTA DE CONTENIDOS DE LOS SUBARCHIVOS (CON CHAR*)
 */
t_list* obtenerBloquesBinarios(const char * rutaDelArchivo);

/*
 * t_list* obtenerBloquesBinarios(const char * rutaDelArchivo);
 * RECIBE UNA RUTA DE UN ARCHIVO DE TEXTO A DIVIDIR,
 * RETORNA UNA LISTA DE CONTENIDOS DE LOS SUBARCHIVOS (CON CHAR*)
 */
t_list* obtenerBloquesTexto(const char * rutaDelArchivo);
int cantidadDeBloques(int tamanio);
char* obtenerPuntero(const char* rutaArchivo);
int tamanioArchivoCerrado(const char* rutaArchivo);
t_list* dividirArchivoBinario(char*puntero,int cantidadDeBloques);
t_list* dividirArchivoTxt(char* p);
char* obtenerContenidoDelArchivo(const char* rutaArchivo);
t_list* listaDeContenidos(t_list* rutasDeArchivos);

#endif /* HEADERS_MANEJODEARCHIVOS_H_ */