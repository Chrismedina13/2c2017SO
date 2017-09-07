/*
 ============================================================================
 Name        : Worker.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "Headers/Worker.h"
#include "Headers/socketsWorker.h"

int main(int argc, char *argv[]) {

	Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);

	printf("Archivo de configuracion puerto DataNode : %i \n", config->puertoDataNode);
	printf("Archivo de configuracion puerto worker : %i \n", config->puertoWorker);
	printf("Archivo de configuracion puerto fileSystem : %i \n", config->puertoFileSystem);
	printf("Archivo de configuracion ip fileSystem : %s \n", config->ipFileSystem);
	printf("Archivo de configuracion nombre nodo : %s \n", config->nombreNodo);
	printf("Archivo de configuracion ruta data.bin : %s \n", config->rutaDataBin);

	//socketServer
	int FDServidor = socketServidor(config->puertoWorker);

	printf("Se conecto un Master su FD es el  = %d\n",FDServidor);

	if(send(FDServidor,"Hola Master",11,0) != -1){

		puts("Mensaje a Master enviado correctamente");
	}
	else{
		puts("Error en el envio");
	}

	free(config); // no estoy seguro si se libera asi
	return EXIT_SUCCESS;
}
