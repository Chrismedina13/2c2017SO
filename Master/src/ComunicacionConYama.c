#include "Headers/ComunicacionConYama.h"
#include "Headers/ComunicacionConWorker.h"
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

	mensajesEnviadosAYama(NOMBRE_ARCHIVO, FDsocketClienteYAMA, job, tamanioJOB);

	free(job);
}
ParametrosComunicacionYAMA* setParametrosComunicacionYAMA(int puerto, char* ip) {

	ParametrosComunicacionYAMA* parametros = malloc(
			sizeof(ParametrosComunicacionYAMA));
	parametros->ipYAMA = ip;
	parametros->puertoYAMA = puerto;
	return parametros;
}

void mensajesEnviadosAYama(int codigo, int FDsocketClienteYAMA, char* mensaje,
		int tamanio) {
	switch (codigo) {
	Paquete* paqueteDeEnvioDeJOB;
case NOMBRE_ARCHIVO:
	paqueteDeEnvioDeJOB = crearPaquete(NOMBRE_ARCHIVO, tamanio, mensaje);

	if (enviarPaquete(FDsocketClienteYAMA, paqueteDeEnvioDeJOB) == -1) {
		logInfo("Error en envio de job");
	}

	destruirPaquete(paqueteDeEnvioDeJOB);
	break;

case FIN_TRANSFORMACION:
	logInfo(
			"Master envia señal de finalización de Transformación(EXITO o FRACASO)");
	Paquete* paqueteTranf = crearPaquete(FIN_TRANSFORMACION, tamanio, mensaje);

	if (enviarPaquete(FDsocketClienteYAMA, paqueteTranf) == -1) {
		logInfo("Error en envio de respuesta de Transformacion.");
	}

	destruirPaquete(paqueteTranf);

	break;

case FIN_REDUCCION_LOCAL:
	logInfo(
			"Master envia señal de finalización de Reducción Local(EXITO o FRACASO)");
	Paquete* paqueteRedLocal = crearPaquete(FIN_REDUCCION_LOCAL, tamanio,
			mensaje);

	if (enviarPaquete(FDsocketClienteYAMA, paqueteRedLocal) == -1) {
		logInfo("Error en envio de respuesta de Red.Local.");
	}

	destruirPaquete(paqueteRedLocal);

	break;

case FIN_REDUCCION_GLOBAL:
	logInfo(
			"Master envia señal de finalización de Reducción Global(EXITO o FRACASO)");

	Paquete* paqueteRedGlobal = crearPaquete(FIN_REDUCCION_LOCAL, tamanio,
			mensaje);

	if (enviarPaquete(FDsocketClienteYAMA, paqueteRedGlobal) == -1) {
		logInfo("Error en envio de respuesta de Red.Global.");
	}

	destruirPaquete(paqueteRedGlobal);

	break;

case ALMACENADO_FINAL:
	logInfo(
			"Master envia señal de finalización de Almacenamiento Final(EXITO o FRACASO).");

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
	char pesoMsj[4];
	int tamanio;
	char* mensaje;

	switch (codigo) {
	case SOL_TRANSFORMACION:
		logInfo("Master recibe de Yama solicitud de transformación.");
		// REcibir paquete de solicitud de transformacion y deserializar

		recv(FDsocketClienteYAMA, pesoMsj, 4, 0);

		tamanio = deserializarINT(pesoMsj);

		mensaje = malloc(tamanio + 1);

		logInfo("Tamanio de lo que recibo %i", tamanio);

		if (recv(FDsocketClienteYAMA, mensaje, tamanio, 0) == -1) {

			logInfo("Error en la recepcion de la Estructura Respuesta Transf.");

		} else {

			t_list* listaDeWorkers =
					deserializarListaRespuestaTransf(mensaje);
			logInfo(
					"Se recibió de forma correcta la a Estructura Respuesta Transf .",
					tamanio);
			/*una vez que MASTER recibe los workers a planificar crea un hilo
			por cada nodo de la lista, es decir por cada woker */
			int i;
			RespuestaTransformacionYAMA* respuesta;
			for (i = 0; list_size(listaDeWorkers); i++) {
				logInfo("Creando hilos para comunicacion con WORKERS.");

					pthread_t hiloWorker;

					ParametrosComunicacionWoker* parametrosWorker = setParametrosComunicacionConWoker(respuesta->puertoWorker,respuesta->ipWorkwer);

					pthread_create(&hiloWorker,NULL,(void*) comunicacionWorkers, parametrosWorker);

					pthread_join(hiloWorker,NULL);


			}
		}


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
