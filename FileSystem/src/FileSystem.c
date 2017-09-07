/*
 ============================================================================
 Name        : FileSystem.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include "Headers/FileSystem.h"

int main(int argc, char *argv[]) {

	Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);

	printf("Archivo de configuracion puerto FILESYSTEM : %i \n", config->puerto);



	return EXIT_SUCCESS;
}
