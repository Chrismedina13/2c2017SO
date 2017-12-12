/*
 * comunicacionConWorkerCliente.c
 *
 *  Created on: 11/12/2017
 *      Author: utnso
 */
#include "Headers/comunicacionConWorkerCliente.h"

void comunicacionConWorkerCliente(ParametrosComunicacionConMaster* parametrosMaster) {

	//Creo Servidor Principal
	int socketWorkerServidor;
	socketWorkerServidor = lib_socketServidor(parametrosMaster->puertoWorker);

	fd_set master;
	fd_set read_fds;
	int fd_max;
	int i;
	int FD_Cliente;
	int bytesRecibidos;
	char buffer[4];

	FD_SET(socketWorkerServidor, &master);
	fd_max = socketWorkerServidor;

	for (;;) {

		read_fds = master;
		if (select(fd_max + 1, &read_fds, NULL, NULL, NULL) == -1) {

			logInfo("Error en el select");
		}
		for (i = 0; i <= fd_max; i++) {

			if (FD_ISSET(i, &read_fds)) {

				if (i == socketWorkerServidor) {

					if ((FD_Cliente = lib_aceptarYRegistrarSocket(
							socketWorkerServidor)) == -1) {

						logInfo("Error en el aceept despues del select");

					} else {
						FD_SET(FD_Cliente, &master);
						if (FD_Cliente > fd_max) {
							fd_max = FD_Cliente;
						}
						logInfo(
								"Nueva conexion del socket cliente Master de FD: %i",
								FD_Cliente);
					}
				} else {

					//Recibo datos de algun cliente
					if ((bytesRecibidos = recv(i, buffer, 4, 0)) <= 0) {
						if (bytesRecibidos == 0) {
							logInfo("Conexion cerrada del FD : %i", i);

						}
						close(i);
						FD_CLR(i, &master);

					} else {
						//logInfo("HOLA");1
						int codigo = deserializarINT(buffer);
						//logInfo(" Worker Recibe de Master: %d", codigo);
						printf(" Worker Recibe de Master: %d", codigo);

						char pesoMensaje[4];
						int tamanio;
						char* mensaje;
						int FD_WORKERCLIENTE = i;


						switch (codigo) {
						case SOLICITUD_ARCHIVO_TEMPORAL:
							//ACA EL WORKER SERVIDOR ENVIA A WORKER CLIENTE EL ARCHIVO TEMPORAL PEDIDO

							logInfo("DataNode Recibe la instruccion 'GetBloque' de FileSystem");



								recv(FD_WORKERCLIENTE,pesoMensaje,4,0);

								tamanio= deserializarINT(pesoMensaje);
								logInfo("tamanio de lo que recibo %i", tamanio);
								mensaje = malloc(tamanio + 1);
								mensaje[tamanio] = '\0';
								recv(FD_WORKERCLIENTE,mensaje, tamanio,0);
								//intRecibido = deserializarINT(mensaje);
								logInfo("Recibo del worker cliente: %s", mensaje);

								char* contenido = obtenerPuntero(mensaje);
								char* nombreArchivoTemporal = obtenerSoloNombreDelArchivo(mensaje);

								tamanio = strlen(contenido)+strlen(nombreArchivoTemporal);
								archivo* archivoTemporal = malloc(tamanio);

								char* archivoTemporalSerializado = serializarArchivo(archivoTemporal);

								mensajesEnviadosAWorkerCliente(SOLICITUD_ARCHIVO_TEMPORAL,FD_WORKERCLIENTE,archivoTemporalSerializado,tamanio);

							break;
						default:
							break;

						}
					}
				}

			}
		}

	}
}

ParametrosComunicacionConMaster* setParametrosComunicacionConWorkerCliente(int puerto) {

	ParametrosComunicacionConMaster* parametros = malloc(
			sizeof(ParametrosComunicacionConMaster));
	parametros->puertoWorker = puerto;
	return parametros;
}

void mensajesEnviadosAWorkerCliente(int codigo, int FDMaster, char* mensaje,
		int tamanio) {
/*
	//Worker recibe de master
	char pesoMensaje[4];

	switch (codigo) {
	case FIN_TRANSFORMACION:
		logInfo(
				"Woker envia señal de finalización de Transformación(EXITO o FRACASO)");
		Paquete* paqueteTranf = crearPaquete(FIN_TRANSFORMACION, tamanio,
				mensaje);

		if (enviarPaquete(FDMaster, paqueteTranf) == -1) {
			logInfo("Error en envio de respuesta de Transformacion.");
		}

		destruirPaquete(paqueteTranf);

		break;
	case FIN_REDUCCION_LOCAL:
		logInfo(
				"Woker envia señal de finalización de Reducción Local(EXITO o FRACASO)");
		Paquete* paqueteRedLocal = crearPaquete(FIN_REDUCCION_LOCAL, tamanio,
				mensaje);

		if (enviarPaquete(FDMaster, paqueteRedLocal) == -1) {
			logInfo("Error en envio de respuesta de Red.Local.");
		}

		destruirPaquete(paqueteRedLocal);

		break;

	case FIN_REDUCCION_GLOBAL:
		logInfo(
				"Worker envia señal de finalización de Reducción Global(EXITO o FRACASO)");

		Paquete* paqueteRedGlobal = crearPaquete(FIN_REDUCCION_LOCAL, tamanio,
				mensaje);

		if (enviarPaquete(FDMaster, paqueteRedGlobal) == -1) {
			logInfo("Error en envio de respuesta de Red.Global.");
		}

		destruirPaquete(paqueteRedGlobal);

		break;
	case ALMACENADO_FINAL:
		logInfo(
				"Worker envia señal de finalización de Almacenamiento Final(EXITO o FRACASO).");

		Paquete* paqueteAlmacenado = crearPaquete(FIN_REDUCCION_LOCAL, tamanio,
				mensaje);

		if (enviarPaquete(FDMaster, paqueteAlmacenado) == -1) {
			logInfo("Error en envio de respuesta de Almacenado Final");
		}

		destruirPaquete(paqueteAlmacenado);

		break;

	default:
		break;
	}
*/
}
