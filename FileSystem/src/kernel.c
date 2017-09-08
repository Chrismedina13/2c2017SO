/*
 ============================================================================
 Name        : kernel.c
 Author      : JuanCarlosMujica
 Version     :
 Copyright   : UTN
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "kernel.h"
#include "handler-user.h"
#include "handler-console.h"
#include "handler-cpu.h"
#include "handler-memory.h"
#include "handler-fileSystem.h"
#include <dc-commons/logger.h>

kernel_struct kernelStruct;

int main(int argc, char *argv[]) {


	Configuration* config = config_with(argc > 1 ? argv[1] : NULL);
	initLogMutex(config->log_file, config->log_program_name,
			config->log_print_console,
			log_level_from_string(config->log_level));

	logInfo(config->log_program_name,"Inicializado proceso kernel");
	initializeStruct(&kernelStruct, config);


	logInfo("Imprimo bienvenida al programa");
	printWelcome();


	logInfo("Inicializando estados y variables compartidas");
	inicializarEstados();
	inicializarVariablesCompartidas(config);
	tablaDeAdminstradoresDeHeap = list_create();
	tamanioDePagina = config->size_pagina;

	inicializarSemaforos(config);

	logInfo("%i semaforo",valoresDelosSemoros[2]);


	/* Parte de creacion de servidor kernel para aceptar conexiones de consolas y cpu */
	logInfo("Inicializando sockets servidor");
	crearSockets(&kernelStruct);


	logInfo("Creando el hilo para mantener CPU's");
	pthread_t hiloCpu;
	pthread_create(&hiloCpu, NULL, (void*) handleCPUs, &kernelStruct);

	logInfo("Creando el hilo para mantener Consolas's");
	pthread_t hiloConsola;
	pthread_create(&hiloConsola, NULL, (void*) handleConsolas, &kernelStruct);



	handleFileSystem(&kernelStruct);
	handleMemoria(&kernelStruct);






	logInfo("Inicia el lector de comandos para el usuario");
	handleUserRequest(&kernelStruct);


	pthread_join(hiloCpu,NULL);
	pthread_join(hiloConsola,NULL);





	return EXIT_SUCCESS;
}

void initializeStruct(kernel_struct* kernelStruct, Configuration* config){
	kernelStruct->config = config;
	inicializarArray(MAX_CPUS, kernelStruct->cpuSockets);
	inicializarArray(MAX_CONSOLAS, kernelStruct->consolaSockets);
	kernelStruct->cpuList = list_create();
	kernelStruct->processList = list_create();
	kernelStruct->socketClientFileSystem = -1;
	kernelStruct->socketClientMemoria = -1;
	kernelStruct->socketServerCPU = -1;
	kernelStruct->socketServerConsola = -1;
	pthread_mutex_init(&(kernelStruct->stdoutMutex),NULL);
	pthread_mutex_init(&(kernelStruct->cpuListMutex),NULL);
	pthread_mutex_init(&(kernelStruct->processListMutex),NULL);
}

void inicializarVariablesCompartidas(Configuration* config){

	variablesCompartidas = malloc(sizeof(int)*config->longitud_shared_vars);
	int i;
	for(i=0;i<kernelStruct.config->longitud_shared_vars;i++){
		variablesCompartidas[i] = 0;
	}
}


void inicializarSemaforos(Configuration* config){
	valoresDelosSemoros = malloc(sizeof(int)*config->longitud_semaforo);
	bloqueadosPorSemaforo= malloc(sizeof(t_queue*)*config->longitud_semaforo);
	int i;
	for(i=0; i<config->longitud_semaforo; i++){
		valoresDelosSemoros[i] = config->sem_init[i];
		bloqueadosPorSemaforo[i] = queue_create();
	}
}

