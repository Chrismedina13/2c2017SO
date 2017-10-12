#include "Headers/ComunicacionConFileSystem.h"
#include "Headers/logYAMA.h"
#include "SO_lib/estructuras.h"

void comunicacionConFileSystem(ParametrosComunicacionConFileSystem* param) {
	//socketClienteParaFileSystem
	char* jobAEjecutar;

	int FDsocketClienteFileSystem;
	FDsocketClienteFileSystem = lib_SocketCliente(param->ip, param->puerto);
	logInfo("SocketCliente = %d \n", FDsocketClienteFileSystem);
	char buffer[13];
	if (recv(FDsocketClienteFileSystem, buffer, 13, 0) != -1) {
		logInfo("Se recibio: %s", buffer);

	}

	sem_wait(&semaforoYAMA);
	jobAEjecutar = retirarJobDeLista();
	logInfo("el job a ejecutar es: %s", jobAEjecutar);
	// Pediria a FS los bloques del job y ejecutaria la planificacion
	int tamanioJOB = strlen(jobAEjecutar);

	logInfo("%i",tamanioJOB);

	Paquete* paqueteDeEnvioDeJOB = crearPaquete(NOMBRE_ARCHIVO, tamanioJOB,
			jobAEjecutar);

	if (enviarPaquete(FDsocketClienteFileSystem, paqueteDeEnvioDeJOB) == -1) {
		logInfo("Error en envio de job");
	}

	logInfo("%i",param->disponibilidadBase);
	logInfo("%s",param->algoritmo);
	destruirPaquete(paqueteDeEnvioDeJOB);

	/* Aca tiene que hacer un recv de FS la lista de bloque que componen al archivo
	   la ubicacion de sus copias y espacio ocupado en el bloque. Empieza la planificacion
	   con esos datos

	   Recibiria
	   BLOQUE | copia 1 | copia 2 | bytes ocupados
	*/

	//Devuelve directamente lo que se le tiene que devover a MAster

/*
t_list* planificar(listaDeWorkersAPlanificar lo que me manda FS,param->algoritmo,param->disponibilidadBase){

		if(string_equals_ignore_case(param->algoritmo,"W-CLOCK")){

			//Planificar con W-CLOCK
		}else{

			//Planificar con CLOCK

		}

	}


}*/

ParametrosComunicacionConFileSystem* setParametrosComunicacionConFileSystem(
		int puerto, char* ip,char* algoritmo,int disponiblidadBase) {
	ParametrosComunicacionConFileSystem* parametros = malloc(
			sizeof(ParametrosComunicacionConFileSystem));
	parametros->ip = ip;
	parametros->puerto = puerto;
	parametros->algoritmo = algoritmo;
	parametros->disponibilidadBase = disponiblidadBase;
	return parametros;
}
