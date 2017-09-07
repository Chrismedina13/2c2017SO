/*
 ============================================================================
 Name        : YAMA.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "Headers/yama.h"

int main(int argc, char *argv[]) {

	Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);
	printf("Archivo de configuracion ip fileSystem : %s \n", config->ipFileSystem);
	printf("Archivo de configuracion puerto fileSystem : %i \n", config->puertoFileSystem);
	printf("Archivo de configuracion Retardo de Planificacion : %i \n", config->retardo);
	printf("Archivo de configuracion Algoritmo de Balanceo : %s \n", config->algoritmo_bal);
	return EXIT_SUCCESS;
}
