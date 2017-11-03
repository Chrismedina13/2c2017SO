#include "Headers/ComunicacionConYama.h"
#include "SO_lib/estructuras.h"

void comunicacionYama(ParametrosComunicacionYAMA* parametros) {

	int FDsocketClienteYAMA;
	FDsocketClienteYAMA = lib_SocketCliente(parametros->ipYAMA,
			parametros->puertoYAMA);

	char* job = malloc(50);

	logInfo("Ingrese JOB a Ejecutar");
	scanf("%s", job);

	logInfo("Se va a ejecutar %s", job);

	int tamanioJOB = strlen(job);

	logInfo("%i", tamanioJOB);

	Paquete* paqueteDeEnvioDeJOB = crearPaquete(NOMBRE_ARCHIVO, tamanioJOB,
			job);

	if (enviarPaquete(FDsocketClienteYAMA, paqueteDeEnvioDeJOB) == -1) {
		logInfo("Error en envio de job");
	}

	destruirPaquete(paqueteDeEnvioDeJOB);
	free(job);
}

ParametrosComunicacionYAMA* setParametrosComunicacionYAMA(int puerto, char* ip) {

	ParametrosComunicacionYAMA* parametros = malloc(
			sizeof(ParametrosComunicacionYAMA));
	parametros->ipYAMA = ip;
	parametros->puertoYAMA = puerto;
	return parametros;
}
void mensajesEnviadosAYama(int codigo, int FDsocketClienteYAMA, char mensaje,
		int tamanio) {
	switch (codigo) {
	case NOMBRE_ARCHIVO:
		Paquete* paqueteDeEnvioDeJOB = crearPaquete(NOMBRE_ARCHIVO, tamanioJOB,
				job);

		if (enviarPaquete(FDsocketClienteYAMA, paqueteDeEnvioDeJOB) == -1) {
			logInfo("Error en envio de job");
		}

		destruirPaquete(paqueteDeEnvioDeJOB);
		break;

	case FIN_TRANSFORMACION:
		logInfo(
				"Master envia señal de finalización de Transformación(EXITO o FRACASO)");
		Paquete* paqueteTranf = crearPaquete(FIN_TRANSFORMACION, tamanio,
				mensaje);

		if (enviarPaquete(FDsocketClienteYAMA, paqueteTranf) == -1) {
			logInfo("Error en envio de respuesta de Transformacion.");
		}

		destruirPaquete(paqueteTranf);

		break;

	case FIN_REDUCCION_LOCAL:
		logInfo("Master envia señal de finalización de Reducción Local(EXITO o FRACASO)");
		Paquete* paqueteRedLocal = crearPaquete(FIN_REDUCCION_LOCAL, tamanio,
				mensaje);

		if (enviarPaquete(FDsocketClienteYAMA, paqueteRedLocal) == -1) {
			logInfo("Error en envio de respuesta de Red.Local.");
		}

		destruirPaquete(paqueteRedLocal);

		break;

	case FIN_REDUCCION_GLOBAL:
		logInfo("Master envia señal de finalización de Reducción Global(EXITO o FRACASO)");

		Paquete* paqueteRedGlobal = crearPaquete(FIN_REDUCCION_LOCAL, tamanio,
				mensaje);

		if (enviarPaquete(FDsocketClienteYAMA, paqueteRedGlobal) == -1) {
			logInfo("Error en envio de respuesta de Red.Global.");
		}

		destruirPaquete(paqueteRedGlobal);

		break;

	case ALMACENADO_FINAL:
		logInfo("Master envia señal de finalización de Almacenamiento Final(EXITO o FRACASO).");

		Paquete* paqueteAlmacenado = crearPaquete(FIN_REDUCCION_LOCAL, tamanio,
				mensaje);

		if (enviarPaquete(FDsocketClienteYAMA, paqueteAlmacenado) == -1) {
			logInfo("Error en envio de respuesta de Almacenado Final");
		}

		destruirPaquete(paqueteAlmacenado);

		break;

	default:
		break;
	}
}
void mensajesRecibidosDeYama(int codigo, int FDsocketClienteYAMA) {
	switch (codigo) {
	case SOL_TRANSFORMACION:
		logInfo("Master recibe de Yama solicitud de transformación.");
		// REcibir paquete de solicitud de transformacion y deserializar
		break;
	case SOL_REDUCCION_LOCAL:
		logInfo("Master recibe de Yama solicitud de Reducción Local.");

		break;
	case SOL_REDUCCION_GLOBAL:
		logInfo("Master recibe de Yama solicitud de Reducción Global");

		break;
	case SOL_ALMACENADO_FINAL:
		logInfo("Master recibe de Yama solicitud de Almacenado Final");
		break;

	}
}
