#include "Headers/ComunicacionConYama.h"
#include "Headers/ComunicacionConWorker.h"
#include "SO_lib/estructuras.h"
#include "pthread.h"

void comunicacionYama(ParametrosComunicacionYAMA* parametros) {

	int FDsocketClienteYAMA;


	FDsocketClienteYAMA = lib_SocketCliente(parametros->ipYAMA,
			parametros->puertoYAMA);

	FD_YAMA=FDsocketClienteYAMA;

	char* job = malloc(50);

	logInfo("Ingrese JOB a Ejecutar");
	scanf("%s", job);

	logInfo("Se va a ejecutar %s", job);

	nro_job= job;
	int tamanioJOB = strlen(job);

	logInfo("%i", tamanioJOB);

	mensajesEnviadosAYama(NOMBRE_ARCHIVO, FDsocketClienteYAMA, job, tamanioJOB);
	//voy a recibir datos de Yama
	char pesoCodigo[4];
	recv(FDsocketClienteYAMA, pesoCodigo, 4, 0);

		int codigo = deserializarINT(pesoCodigo);

		logInfo("Recibi de YAMA: %i", codigo);

	mensajesRecibidosDeYama(codigo, FDsocketClienteYAMA);


	char peso[4];
	recv(FDsocketClienteYAMA, peso, 4, 0);

		int cod = deserializarINT(peso);

		logInfo("Recibi de YAMA: %i", cod);

	mensajesRecibidosDeYama(cod, FDsocketClienteYAMA);



	free(job);
}
ParametrosComunicacionYAMA* setParametrosComunicacionYAMA(int puerto, char* ip) {

	ParametrosComunicacionYAMA* parametros = malloc(
			sizeof(ParametrosComunicacionYAMA));
	parametros->ipYAMA = ip;
	parametros->puertoYAMA = puerto;
	return parametros;
}

void mensajesEnviadosAYama(int codigo, int FDsocketClienteYAMA, char* mensaje,int tamanio) {
	switch (codigo) {
	Paquete* paquete;


	case NOMBRE_ARCHIVO:
		paquete = crearPaquete(NOMBRE_ARCHIVO, tamanio, mensaje);

		if (enviarPaquete(FDsocketClienteYAMA, paquete) == -1) {
			logInfo("Error en envio de job");
		}

		destruirPaquete(paquete);
		break;

	case FIN_TRANSFORMACION:
		logInfo(
				"Master envia señal de finalización de Transformación(EXITO o FRACASO)");
		paquete = crearPaquete(FIN_TRANSFORMACION, tamanio, mensaje);

		if (enviarPaquete(FDsocketClienteYAMA, paquete) == -1) {
			logInfo("Error en envio de respuesta de Transformacion.");
		}

		destruirPaquete(paquete);

		break;

	case FIN_REDUCCION_LOCAL:
		logInfo(
				"Master envia señal de finalización de Reducción Local(EXITO o FRACASO)");
		paquete = crearPaquete(FIN_REDUCCION_LOCAL, tamanio,
				mensaje);

		if (enviarPaquete(FDsocketClienteYAMA, paquete) == -1) {
			logInfo("Error en envio de respuesta de Red.Local.");
		}

		destruirPaquete(paquete);

		break;

	case FIN_REDUCCION_GLOBAL:
		logInfo(
				"Master envia señal de finalización de Reducción Global(EXITO o FRACASO)");

		paquete = crearPaquete(FIN_REDUCCION_LOCAL, tamanio,
				mensaje);

		if (enviarPaquete(FDsocketClienteYAMA, paquete) == -1) {
			logInfo("Error en envio de respuesta de Red.Global.");
		}

		destruirPaquete(paquete);

		break;

	case ALMACENADO_FINAL: // termino bien
		logInfo(
				"Master envia señal de finalización de Almacenamiento Final(EXITO o FRACASO).");

		paquete = crearPaquete(FIN_REDUCCION_LOCAL, tamanio,
				mensaje);

		if (enviarPaquete(FDsocketClienteYAMA, paquete) == -1) {
			logInfo("Error en envio de respuesta de Almacenado Final");
		}

		destruirPaquete(paquete);

		break;
	case ABORTO_JOB: // termino bien
		logInfo(
				"Master envia señal de ABORTO DE JOB");

		paquete = crearPaquete(FIN_REDUCCION_LOCAL, tamanio,
				mensaje);

		if (enviarPaquete(FDsocketClienteYAMA, paquete) == -1) {
			logInfo("Error en envio de respuesta de Almacenado Final");
		}

		destruirPaquete(paquete);

		break;


	default:
		break;
	}
}
void mensajesRecibidosDeYama(int codigo, int FDsocketClienteYAMA) {
	char pesoMsj[4];
	int tamanio;
	char* mensaje;
	t_list* listaDeWorkers = list_create();
	int numeroDeJob;

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

			logInfo("Recibo corecctamente la respuesta");

			 listaDeWorkers = deserializarListaYAMA(mensaje);

			 logInfo("Terminada la deserializacion");
			int i;
			RespuestaTransformacionYAMA* respuesta;

			tamanioLista = list_size(listaDeWorkers);

			RespuestaTransformacionYAMA* ordenoPorNodo(RespuestaTransformacionYAMA *nodo1, RespuestaTransformacionYAMA *nodo2) {
						if (nodo1->nodo > nodo2->nodo) {
							return nodo2;
						} else {
							return nodo1;
						}
					}

			//ordeno la lista de los workers por nodo para crear un hilo por cada worker a ejecutar
			list_sort(listaDeWorkers,ordenoPorNodo);

			for (i = 0; i<tamanioLista; i++) {

				respuesta = list_get(listaDeWorkers, i);
				int nodoini,nodoAux;
				respuesta->nodo = nodoini;

				if(nodoini != nodoAux){
					logInfo("Creando hilos para comunicacion con WORKERS.");

					pthread_t hiloWorker;

					ParametrosComunicacionWoker* parametrosWorker =
							setParametrosComunicacionConWoker(
									respuesta->puertoWorker,
									respuesta->ipWorkwer, respuesta->nodo,
									respuesta->archivoTemporal,
									respuesta->bytesOcupados,
									respuesta->bloque);

					pthread_create(&hiloWorker, NULL,
							(void*) comunicacionWorkers, parametrosWorker);

					pthread_join(hiloWorker, NULL);

					nodoAux = nodoini;
				}else{
					i++;
				}
			}
		}

		break;
	case SOL_REDUCCION_LOCAL:
		logInfo("Master recibe de Yama solicitud de Reducción Local.");

		break;
	case NUMERO_DE_JOB:
		logInfo("Master recibe el numero De JOB.");

		recv(FDsocketClienteYAMA, pesoMsj, 4, 0);

		tamanio = deserializarINT(pesoMsj);

		mensaje = malloc(tamanio + 1);

		logInfo("Tamanio de lo que recibo %i", tamanio);

		if (recv(FDsocketClienteYAMA, mensaje, tamanio, 0) == -1) {

		logInfo("Error en la recepcion de la Estructura Respuesta Transf.");

		} else {

			numeroDeJob = deserializarINT(mensaje);
			logInfo("El job asignado es %i",numeroDeJob);
			nro_job=numeroDeJob;
		}

		break;

	case SOL_REDUCCION_GLOBAL:
		logInfo("Master recibe de Yama solicitud de Reducción Global");

		break;
	case SOL_ALMACENADO_FINAL:
		logInfo("Master recibe de Yama solicitud de Almacenado Final");
		break;

	}
}
