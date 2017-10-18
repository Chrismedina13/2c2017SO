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


	//PRUEBA DE MMAP FUNCIONANDO!!!!
/*
	char* p;
	const char* nombreDelArchivo = "/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/FileSystem/archivoprueba.txt";
	int respuesta= pasarAMemoria(nombreDelArchivo,&p); //PASAMOS POR REFERENCIA EL PUNTERO QUE APUNTA A MEMORIA
	if(!respuesta) printf("\n¡Paso a memoria exitosamente!\n\n");
	else printf("\nError al pasar archivo a memoria\n\n");
*/


	//Archivo de logs
	crearLog("FileSystem.log", "FS", 1, log_level_from_string("INFO"));

	//PRUEBA DE ESCTRUCTURAS FS
	//cargar directorios
	cargarDirectorios();

	//cantidad directorios
	int cantDir = cantidadDirectorios();
	logInfo("el index del ultimo directorio es: %d \n", cantDir);

	//existe directorio
	char* nombreDir = "yamafs";
	int padreDir = 0;
	int existe = existeDirectorio(nombreDir, padreDir);
	if(existe == 1)	logInfo("el directorio %s existe.\n", nombreDir);
	else logInfo("el directorio %s no existe.\n", nombreDir);


	//Configuracion
	Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);

	logInfo(
			"Archivo de configuracion PUERTO FILE SYSTEM PARA RECIBIR DATA NODE : %i \n",
			config->puerto_dn);
	logInfo(
			"Archivo de configuracion PUERTO FILE SYSTEM PARA RECIBIR YAMA : %i \n",
			config->puerto_yama);


logInfo("Creando estructuras Administrativas");

//bitarray_create(bloques,20*4);
//inicializarBitmap(bloques);
//imprimirResultado(bloques);







	logInfo("Creando el hilo para comunicarme con Data Node");
	logInfo("Creando el hilo para comunicarme con YAMA");
	logInfo("Creando el hilo para comunicarme con WORKER");

	ParametrosComunicacion* parametros = setParametrosComunicacion(config->puerto_dn, config->puerto_yama,config->puerto_worker); // Hay que agregar el Puerto de Worker

	pthread_t hiloDN, hiloYAMA,hiloWorker;

	pthread_create(&hiloDN, NULL, (void*) comunicacionDN, parametros);
	pthread_create(&hiloYAMA, NULL, (void*) comunicacionYAMA, parametros);
	pthread_create(&hiloWorker, NULL, (void*) comunicacionWorker, parametros);

	//consolaFileSystem();

	pthread_join(hiloWorker, NULL);
	pthread_join(hiloYAMA, NULL);
	pthread_join(hiloDN, NULL);



	//list_remove_and_destroy_element(ubiacionBloquesArchivo);
	//list_remove_and_destroy_element(nodos);
	//list_remove_and_destroy_element(bitmapNodos);


		free(config);
	return EXIT_SUCCESS;
}
