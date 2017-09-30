#include "Headers/ComunicacionConYama.h"
#include "SO_lib/estructuras.h"


void comunicacionYama(ParametrosComunicacionYAMA* parametros) {

	int FDsocketClienteYAMA;
	FDsocketClienteYAMA = lib_SocketCliente(parametros->ipYAMA, parametros->puertoYAMA);

	char* job = malloc(50);

	logInfo("Ingrese JOB a Ejecutar");
	scanf("%s", job);

	logInfo("Se va a ejecutar %s", job);

	int tamanioJOB = strlen(job);

	logInfo("%i",tamanioJOB);

	Paquete* paqueteDeEnvioDeJOB = crearPaquete(NOMBRE_ARCHIVO,tamanioJOB,job);

	if(enviarPaquete(FDsocketClienteYAMA,paqueteDeEnvioDeJOB) == -1){
		logInfo("Error en envio de job");
	}

	destruirPaquete(paqueteDeEnvioDeJOB);
	free(job);
}

ParametrosComunicacionYAMA* setParametrosComunicacionYAMA(int puerto,char* ip){

	ParametrosComunicacionYAMA* parametros = malloc(sizeof(ParametrosComunicacionYAMA));
	parametros->ipYAMA = ip;
	parametros->puertoYAMA = puerto;
	return parametros;
}
