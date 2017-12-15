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
		}else{
			logInfo("Se envio respuesta de Transformacion a YAMA exitosamente.");
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

			    int indiceAGuardar = 0;
			    int i;
			    int indice;
			    int cantidad =cantidadDeNodos(listaDeWorkers);
			    ParametroParaWorker vectorParam[cantidad];

			    for (i = 0 ; i < list_size (listaDeWorkers); i ++) {
			            RespuestaTransformacionYAMA* respuesta = list_get(listaDeWorkers,i);
			            int indice = indiceDelVector(vectorParam,cantidad,respuesta);
			            if(indice>=0){
			                //CREO LA ESTRUCTURA QUE VA A CONTENER LA LISTA DE TRANSFORMACIOIONES
			                tareaTransformacion* tarea = malloc(sizeof(int)*2 + strlen(respuesta->archivoTemporal));
			                tarea->bloque = respuesta->bloque;
			                tarea->bytesOcupados = respuesta->bytesOcupados;
			                tarea->archivoTemporal = respuesta->archivoTemporal;

			                //AGREGO LA ESTRUCTURA A LA LISTA DE TAREAS DEL NODO
			                list_add(vectorParam[indice].transformaciones,tarea);

			            }
			            else{
			                //CREO LA ESTRUCTURA PARA EL VECTOR

			                vectorParam[indiceAGuardar].nodo = respuesta->nodo;
			                vectorParam[indiceAGuardar].puerto = respuesta->puertoWorker;
			                vectorParam[indiceAGuardar].ip = respuesta->ipWorkwer;
			                vectorParam[indiceAGuardar].transformaciones = list_create();

			                //CREO LA ESTRUCTURA QUE VA A CONTENER LA LISTA DE TRANSFORMACIOIONES
			                tareaTransformacion* tarea = malloc(sizeof(int)*2 + strlen(respuesta->archivoTemporal));
			                tarea->bloque = respuesta->bloque;
			                tarea->bytesOcupados = respuesta->bytesOcupados;
			                tarea->archivoTemporal = respuesta->archivoTemporal;

			                //AGREGO LA ESTRUCTURA A LA LISTA DE TAREAS DEL NODO
			                list_add(vectorParam[indiceAGuardar].transformaciones,tarea);

			                indiceAGuardar++;

			            }
			    }

			    ///mostrar lo que cargo en parametrosparaworker
			    int j= 0;
			    int k = 0;
			    int elementos;
			    while(j<cantidad){
			        logInfo("IP: %s\nPuerto: %d\nNodo: %d\n",vectorParam[j].ip,vectorParam[j].puerto,vectorParam[j].nodo);
			        t_list* lista = vectorParam[j].transformaciones;
			        elementos = list_size(lista);
			        k = 0;
			        while(k<elementos)
			        {
			            tareaTransformacion* p = list_get(lista,k);
			            printf("Bloque: %d | Bytes ocupados: %d | Archivo temporal: %s\n\n",p->bloque,p->bytesOcupados,p->archivoTemporal);
			            k++;
			        }
			        j++;
			    }
			    //////////////////////////////////
			    int a;
			    for(a=0;a<cantidad;a++){
			    	pthread_t hiloWorker;

					ParametrosComunicacionWoker* parametrosWorker =
							setParametrosComunicacionConWoker(
									vectorParam[a].puerto,
									vectorParam[a].ip,
									vectorParam[a].nodo,
									vectorParam[a].transformaciones);

					pthread_create(&hiloWorker, NULL,(void*) comunicacionWorkers, parametrosWorker);

					pthread_join(hiloWorker, NULL);

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
