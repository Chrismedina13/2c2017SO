#include "Headers/FileSystem.h"
#include "commons/string.h"
#include "Headers/consola.h"
#include "Headers/comunicacionConYama.h"
#include "Headers/comunicacionConDN.h"
#include <stdbool.h>
#include <pthread.h>
#include "Headers/comunicacionConWorker.h"
#include "Headers/pruebas.h"

int main(int argc, char *argv[]) {

	//puntero para tabla de archivos


	int max_archivos = MAX;

	tabla_archivos *archivosPtr = malloc (max_archivos * sizeof (tabla_archivos));

	//estructuras para manejo de archivos


	//probando funciones de manejo de archivo

	int tamanio_registro_archivos = sizeof(tabla_archivos);
	printf("%d /n", tamanio_registro_archivos);

	//PRUEBA DE MMAP FUNCIONANDO!!!!
/*

	const char* nombreDelArchivoTxt = "/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/FileSystem/archivoprueba.txt";
	const char* nombreDelArchivoBin = "/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/FileSystem/archivoprueba.bin";

 //¡BINARIO!
	int pasoAMemoriaBinario = pasarAMemoriaBinario(nombreDelArchivoBin);
	if(pasoAMemoriaBinario == 0) printf("\nPaso a memoria y division de archivo correcto\n\n");
	else printf("\nError al pasar a memoria\n\n");
//FIN BINARIO

//¡TXT!
	int pasoAMemoriaTxt = pasarAMemoriaTxt(nombreDelArchivoTxt);
	if(pasoAMemoriaTxt == 0) printf("\nPaso a memoria y division de archivo correcto\n\n");
	else printf("\nError al pasar a memoria\n\n");
//FIN TXT.

*/
	//Archivo de logs
	crearLog("FileSystem.log", "FS", 1, log_level_from_string("INFO"));


	//Configuracion
	Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);

	logInfo(
			"Archivo de configuracion PUERTO FILE SYSTEM PARA RECIBIR DATA NODE : %i \n",
			config->puerto_dn);
	logInfo(
			"Archivo de configuracion PUERTO FILE SYSTEM PARA RECIBIR YAMA : %i \n",
			config->puerto_yama);


logInfo("Creando estructuras Administrativas");


	logInfo("Creando el hilo para comunicarme con Data Node");

	logInfo("Creando el hilo para comunicarme con YAMA");
	logInfo("Creando el hilo para comunicarme con WORKER");

	ParametrosComunicacion* parametros = setParametrosComunicacion(config->puerto_dn, config->puerto_yama,config->puerto_worker); // Hay que agregar el Puerto de Worker

	pthread_t hiloConsola, hiloDN, hiloYAMA,hiloWorker;

	pthread_create(&hiloDN, NULL, (void*) comunicacionDN, parametros);
	pthread_create(&hiloConsola, NULL, (void*) consolaFileSystem, NULL);


	int estado_seguro=false; //estado_seguro va a estar en true una vez que los nodos tengan el arch y las estructuras esten actualizadas
    if(estado_seguro){
	logInfo("file system en estado seguro. Me puedo conectar con YAMA");
	pthread_create(&hiloYAMA, NULL, (void*) comunicacionYAMA, parametros);
    }else{
    	logInfo(" FILE SYSTEM NO ESTA EN ESTADO SEGURO. NO ME PUEDO COMUNICAR CON YAMA");
    }

	pthread_create(&hiloWorker, NULL, (void*) comunicacionWorker, parametros);

	//consolaFileSystem();

	pthread_join(hiloWorker, NULL);
	pthread_join(hiloYAMA, NULL);
	pthread_join(hiloDN, NULL);
	pthread_join(hiloConsola, NULL);


	//list_remove_and_destroy_element(ubiacionBloquesArchivo);
	//list_remove_and_destroy_element(nodos);
	//list_remove_and_destroy_element(bitmapNodos);


		free(config);
	return EXIT_SUCCESS;
}
