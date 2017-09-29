#include "Headers/ComunicacionConYama.h"
#include "SO_lib/estructuras.h"


void comunicacionYama(ParametrosComunicacionYAMA* parametros) {

	Paquete* paqueteDeEnvioDeJOB;

	int FDsocketClienteYAMA;
	FDsocketClienteYAMA = lib_SocketCliente(parametros->ipYAMA, parametros->puertoYAMA);

	char job[100];

	logInfo("Ingrese JOB a Ejecutar");
	scanf("%c", job);

	logInfo("Se va a ejecutar %s", job);

	int tamanioJOB = strlen(job);

	paqueteDeEnvioDeJOB = crearPaquete(NOMBRE_ARCHIVO,tamanioJOB,job);

	if(enviarPaquete(FDsocketClienteYAMA,paqueteDeEnvioDeJOB) == -1){
		logInfo("Error en envio de job");
	}

	destruirPaquete(paqueteDeEnvioDeJOB);

}

ParametrosComunicacionYAMA* setParametrosComunicacionYAMA(int puerto,char* ip){

	ParametrosComunicacionYAMA* parametros = malloc(sizeof(ParametrosComunicacionYAMA));
	parametros->ipYAMA = ip;
	parametros->puertoYAMA = puerto;
	return parametros;
}
