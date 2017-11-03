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


t_list* obtenerBloquesBinarios(const char * rutaDelArchivo);
t_list* obtenerBloquesTexto(const char * rutaDelArchivo);
int cantidadDeBloques(int tamanio);
char* obtenerPuntero(const char* rutaArchivo);
int tamanioArchivoCerrado(const char* rutaArchivo);
t_list* dividirArchivoBinario(char*puntero,int cantidadDeBloques);
t_list* dividirArchivoTxt(char* p);
char* obtenerContenidoDelArchivo(const char* rutaArchivo);
t_list* listaDeContenidos(t_list* rutasDeArchivos);

#endif /* HEADERS_MANEJODEARCHIVOS_H_ */
