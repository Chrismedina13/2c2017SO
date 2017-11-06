#include "Headers/FileSystem.h"
#include "commons/string.h"
#include "Headers/consola.h"
#include "Headers/comunicacionConYama.h"
#include "Headers/comunicacionConDN.h"
#include <stdbool.h>
#include <pthread.h>
#include "Headers/comunicacionConWorker.h"
#include "Headers/nodos.h"

int main(int argc, char *argv[]) {
/*
	tabla_de_nodos.tamanio=40;
	tabla_de_nodos.bloqueslibres=10;
	tabla_de_nodos.listaNodos = list_create();
	tabla_de_nodos.listaCapacidadNodos = list_create();

	int* numero1 = 1;
	int* numero2 = 2;
	list_add(tabla_de_nodos.listaNodos,numero1);
	list_add(tabla_de_nodos.listaNodos,numero2);

	bloques_nodo* nodo1 = malloc(sizeof(bloques_nodo));
	nodo1->nodo=1;
	nodo1->bloquestotales=20;
	nodo1->bloqueslibres=5;

	list_add(tabla_de_nodos.listaCapacidadNodos,nodo1);

	bloques_nodo* nodo2 = malloc(sizeof(bloques_nodo));;
	nodo2->nodo=2;
	nodo2->bloquestotales=20;
	nodo2->bloqueslibres=5;

	list_add(&(tabla_de_nodos.listaCapacidadNodos),nodo2);

	int resp = crearRegistroArchivoNodos(tabla_de_nodos);
	if(resp==0) printf("\narchivo creado correctamente\n");
	else printf("\narchivo no creado.\n");
*/
	//Archivo de logs
	crearLog("FileSystem.log", "FS", 1, log_level_from_string("INFO"));

	//probando probando

	crearRegistroArchivo("/home/utnso/ejemplo.txt", "ejemplo.txt", "TEXTO", 3);

	int resultado = pathToIndex("yamafs/metadata/archivos/3/ejemplo.txt");
	printf("%d", resultado);

	//eliminarArchivo("yamafs/metadata/archivos/3/ejemplo.txt");

	//rename("yamafs/metadata/archivos/3/ejemplo.txt","yamafs/metadata/archivos/3/ejemplo2.txt");
	//cambiarNombreArchivo("yamafs/metadata/archivos/3/ejemplo.txt", "otroejemplo.txt");

	//Configuracion
	Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);

	logInfo(
			"Archivo de configuracion PUERTO FILE SYSTEM PARA RECIBIR DATA NODE : %i \n",
			config->puerto_dn);
	logInfo(
			"Archivo de configuracion PUERTO FILE SYSTEM PARA RECIBIR YAMA : %i \n",
			config->puerto_yama);


	if(config->estado_recuperacion==0){
		logInfo( "FILE SYSTEM NO SE ENCUENTRA EN ESTADO DE RECUPERACION");
		logInfo("CREANDO ESTRUCTURAS ADMINISTRATIVAS");




	}else{
		logInfo("FILE SYSTEM SE ENCUENTRA EN ESTADO DE RECUPERACION");
		logInfo("LEVANTANDO ESTRUCTURAS DEL ESTADO ANTERIOR...");

		//ACA ES DONDE DESDE NODOS.BIN, DIRECTORIOS.DAT Y ARCHIVOS.DAT LEVANTO MIS ESTRUCTURAS.
		//HAY QUE VER SI ME FALTA INFO LE PREGUNTO A YAMA (?)
	}


/*
 * Crear las esctructuras
 * Se conectan los Datanode
 *Romper el archivo
 *Divide los bloques en los distintos nodos que se hayan conectado
 *FS esta en estado seguro, deja conectar a YAMA
 *Queda esperando los pedidos del YAMA
 *
 *
 *
 *
 *Se conecta un dataNode
 *Le pido su id
 *Lo guardo en tabla de no
 */




	logInfo("Creando el hilo para comunicarme con Data Node");

	logInfo("Creando el hilo para comunicarme con YAMA");
	logInfo("Creando el hilo para comunicarme con WORKER");

	ParametrosComunicacion* parametros = setParametrosComunicacion(config->puerto_dn, config->puerto_yama,config->puerto_worker); // Hay que agregar el Puerto de Worker

	pthread_t hiloConsola, hiloDN, hiloYAMA,hiloWorker;

	pthread_create(&hiloDN, NULL, (void*) comunicacionDN, parametros);
	pthread_create(&hiloConsola, NULL, (void*) consolaFileSystem, NULL);
	pthread_create(&hiloYAMA, NULL, (void*) comunicacionYAMA, parametros);
	pthread_create(&hiloWorker, NULL, (void*) comunicacionWorker, parametros);



	pthread_join(hiloWorker, NULL);
	pthread_join(hiloYAMA, NULL);
	pthread_join(hiloConsola, NULL);
	pthread_join(hiloDN, NULL);




		free(config);
	return EXIT_SUCCESS;
}
