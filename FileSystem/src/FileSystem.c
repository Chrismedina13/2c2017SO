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

	//Archivo de logs
	crearLog("FileSystem.log", "FS", 1, log_level_from_string("INFO"));

	//probando
	t_list* nodos = cargarNodos(); //bloques_nodo

	int status;

	t_list* Mapa_de_bits = tablaNodosToNodos(tabla_de_nodos.listaNodos);

	showBitMap(Mapa_de_bits);

	//comandos[1] ruta local
	//comandos[2] ruta yamafs

	//parte el archivo en bloques

	t_list* bloques = obtenerBloquesTexto("/home/utnso/ejemplo.txt"); //quedan cargados en bloques

	//le pasa los bloques a los nodos

	t_list* ubicaciones; //tipo ubicacionBloquesArchivo
	ubicaciones = distribuirBloques(bloques, Mapa_de_bits);

	//crea registro del archivo en YAMAFS

	status = crearRegistroArchivo("/home/utnso/ejemplo.txt","/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/FileSystem/yamafs/metadata/archivos/4/ejemplo.txt", ubicaciones);
	if(status==1){
		logInfo("Registro de archivo creado correctamente.");
	}
	if(status==1){
		logInfo("Registro de archivo no pudo ser creado.");
	}


	//Configuracion
	Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);

	logInfo(
			"Archivo de configuracion PUERTO FILE SYSTEM PARA RECIBIR DATA NODE : %i \n",
			config->puerto_dn);
	logInfo(
			"Archivo de configuracion PUERTO FILE SYSTEM PARA RECIBIR YAMA : %i \n",
			config->puerto_yama);


	if(config->estado_recuperacion==0){
		logInfo("FILE SYSTEM NO SE ENCUENTRA EN ESTADO DE RECUPERACION");
		logInfo("CREANDO ESTRUCTURAS ADMINISTRATIVAS");
	}

	else{
		logInfo("FILE SYSTEM SE ENCUENTRA EN ESTADO DE RECUPERACION");
		logInfo("LEVANTANDO ESTRUCTURAS DEL ESTADO ANTERIOR...");

		//ACA ES DONDE DESDE NODOS.BIN, DIRECTORIOS.DAT Y ARCHIVOS.DAT LEVANTO MIS ESTRUCTURAS. --> me parece que levanto las estructuras cuando en la consola se pone format
		//HAY QUE VER SI ME FALTA INFO LE PREGUNTO A YAMA (?)
	}

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

	//recibir datos de nodos (dataNode)

	//recibirNodos();
	//cargarNodos();//ejemplo hardcodeado de 2 nodos, para probar las funciones de consola

	//deja que se conecte yama, y se queda esperando


	free(config);
	return EXIT_SUCCESS;
}
