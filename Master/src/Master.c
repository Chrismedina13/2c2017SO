/*
 ============================================================================
 Name        : Master.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include "Headers/Master.h"

#include "Headers/ComunicacionConYama.h"

int main(int argc, char *argv[]) {

	ruta_transformador= "/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/Master/scripts/transformador.py";
	ruta_reduccion ="/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/Master/scripts/reductor.py";

	crearLog("Master.log", "MASTER", 1, log_level_from_string("INFO"));

	Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);

	logInfo("Archivo de configuracion IP YAMA : %s \n", config->ipYama);
	logInfo("Archivo de configuracion YAMA PUERTO : %i \n", config->puertoYama);

	logInfo("Creando el hilo para comunicarme con Yama");



 	ParametrosComunicacionYAMA* parametrosYAMA = setParametrosComunicacionYAMA(config->puertoYama,config->ipYama);

	pthread_t hiloYama;
	pthread_create(&hiloYama, NULL, (void*) comunicacionYama, parametrosYAMA);

	pthread_join(hiloYama,NULL);

	free(config);
	return EXIT_SUCCESS;
}
