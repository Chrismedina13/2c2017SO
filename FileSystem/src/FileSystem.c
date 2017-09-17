#include "Headers/FileSystem.h"
#include "commons/string.h"
#include "Headers/consola.h"
#include "Headers/comunicacionConYama.h"
#include "Headers/comunicacionConDN.h"
#include <stdbool.h>
#include <pthread.h>

int main(int argc, char *argv[]) {

	//Archivo de logs
	crearLog("FileSystem.log", "FS", 1, log_level_from_string("INFO"));

	//Configuracion
	Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);

	printf("Archivo de configuracion puerto FILESYSTEM : %i \n",
			config->puerto1);
	printf("Archivo de configuracion puerto FILESYSTEM : %i \n",
			config->puerto2);


	//logInfo("Archivo de configuracion PUERTO FILE SYSTEM PARA RECIBIR DATA NODE : %s \n", config->puerto1);
	//logInfo("Archivo de configuracion PUERTO FILE SYSTEM PARA RECIBIR YAMA : %i \n", config->puerto2);

	//logInfo("Creando el hilo para comunicarme con Data Node");
	//logInfo("Creando el hilo para comunicarme con YAMA");



	ParametrosComunicacion* parametros = setParametrosComunicacion(config->puerto2, config->puerto1);
	pthread_t hiloDN, hiloYAMA;

	pthread_create(&hiloDN, NULL, (void*) comunicacionDN, parametros);
	pthread_create(&hiloYAMA, NULL, (void*) comunicacionYAMA, parametros);

	consolaFileSystem();


	pthread_join(hiloYAMA,NULL);
	pthread_join(hiloDN,NULL);


	free(config);



	return EXIT_SUCCESS;
}
