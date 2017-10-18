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
/*pasa a memoria el archivo que tiene como parametro y su direccion se lo asigna por referencia al parametro puntero
y devuelve el tamaño del archivo.*/
int cantidadDeBloques(int tamanio); // Devuelve la cantidad de bloques que se pueden hacer con 1MB
void mostrarBloque(char*puntero,int numeroDeBloque,int cantidadDeBloques); //Muestra por pantalla el bloque que esta en la posicion que le digamos.
