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

int pasarAMemoria (const char* rutaArchivo);
/*pasa a memoria el archivo que tiene como parametro y devuelve el tamaño del archivo.*/
int cantidadDeBloques(int tamanio); // Devuelve la cantidad de bloques que se pueden hacer con 1KB
t_list* dividirArchivo(char*puntero,int cantidadDeBloques); //Separo el contenido del archivo pasado a memoria en la cantidad de bloques que le diga.
int imprimirArchivo(t_list* lista);
