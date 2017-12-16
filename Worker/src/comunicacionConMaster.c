/*
 * socketsWorker.c
 *
 *  Created on: 6/9/2017
 *      Author: utnso
 */
#include "Headers/comunicacionConMaster.h"

#define MAX 100

void comunicacionConMaster(ParametrosComunicacionConMaster* parametrosMaster) {

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

						int codigo = deserializarINT(buffer);
						logInfo(" Worker Recibe de Master: %d", codigo);

						char pesoMensaje[4];
						int tamanio;
						char* mensaje;
						int FDMaster = i;

						resultadoJob* resultado_job;
						char* mensajeAEnviar;

						switch (codigo) {
						case SCRIPT_TRANSFORMADOR:
							//RECIBO EL SCRIPT
							recv(FDMaster, pesoMensaje, 4, 0);
							tamanio = deserializarINT(pesoMensaje);
							mensaje = malloc(tamanio);
							mensaje[tamanio] = '\0';

							if (recv(FDMaster, mensaje, tamanio, 0) == -1) {

								logInfo(
										"Error al recibir el script transformador.");
							} else {

								scriptTransformador = deserilizarScript(mensaje);
								//logInfo("scriptTransformador %s",scriptTransformador->nombre);
								//logInfo("scriptTransformador %s",scriptTransformador->contenido);

								rearmar_script(scriptTransformador, SCRIPT_TRANSFORMADOR);
								logInfo("Se recibio correctamente el script transformador.\n\n");
							}
							break;
						case SOL_TRANSFORMACION:
							recv(FDMaster, pesoMensaje, 4, 0);
							tamanio = deserializarINT(pesoMensaje);
							mensaje = malloc(tamanio);
							mensaje[tamanio] = '\0';
							if (recv(FDMaster, mensaje, tamanio, 0) == -1) {
								logInfo(
										"Error en la recepcion de Info de Master.");
							} else {
								infoTransformacionParaWorker * info =deserializarInfoParaWorker(mensaje);
								int bloque = info->bloque;
								char* archivoTemporal = info->archivoTemporal;
								logInfo("Archivo temporal: %s",archivoTemporal);
								//EJECUTO EL SCRIPT EN EL BLOQUE INDICADO
								logInfo("Bloque %i",bloque);
								char* contenidoBloque = get_bloque(0);
								int tamanio = tamanioArchivoCerrado("/home/utnso/Escritorio/archivo.txt"); //PARA PROBBAR EL TAMAÑO
								char* contenido = string_substring(contenidoBloque,0,tamanio);

								logInfo("Llego hasta acaaa");
								char* nombreArchivoBloque ="/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/Worker/tmp/archivoBloque.txt";
								crearArchivo(contenido, nombreArchivoBloque);
								int estado = ejecutarScriptTransformador(rutaScriptTransformador, nombreArchivoBloque,archivoTemporal);
								logInfo("estado %i",estado);
								//Estado 0 significa que lo hizo bien

								//BORRO EL ARCHIVO TEMPORAL archivoBloque Y EL SCRIPT
								//destruirArchivoOScript(nombreArchivoBloque);
								//destruirArchivoOScript(rutaScriptTransformador);

								//ENVÍO EL ESTADO DE LA EJECUCION DEL SCRIPT TRANSFORMADOR A MASTER
								resultado_job = malloc(sizeof(int) * 2);
								resultado_job->nodo = id_nodo;
								resultado_job->resultado = estado;
								mensajeAEnviar = serializarResultado(resultado_job);
								int tamanio1 =sizeof(int) * 2;
								mensajesEnviadosAMaster(FIN_TRANSFORMACION,	FDMaster, mensajeAEnviar,	tamanio1);
								logInfo("Estado de transformacion enviada a master.\n\n");
							}

							break;

						case SOL_REDUCCION_LOCAL:
							recv(FDMaster, pesoMensaje, 4, 0);
							tamanio = deserializarINT(pesoMensaje);
							mensaje = malloc(tamanio);
							mensaje[tamanio] = '\0';
							if (recv(FDMaster, mensaje, tamanio, 0) == -1) {
								logInfo(
										"Error en la recepcion de Info de Master.");
							} else {
								infoReduccionLocalParaWorker* info =
										deserializarinfoReduccionLocalParaWorker(
												mensaje);
								char* archivoApareado =
										"/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/Worker/tmp/localApareado";

								//REARMO EL SCRIPT
								rearmar_script(&(info->scriptReduccionLocal),
								REDUCCION_LOCAL);

								//APAREO LOS ARCHIVOS TEMPORALES TRANSFORMADOS QUE VOY A REDUCIR
								apareoDeArchivosVectores(
										info->listaDeArchivosTemporales,
										archivoApareado);

								//EJECUTO EL SCRIPT AL SCRIPT
								int estado = ejecutarScriptReductor(
										info->scriptReduccionLocal.nombre,
										archivoApareado,
										info->archivoTemporalReduccionLocal);

								//BORRO EL ARCHIVO TEMPORAL archivoBloque Y EL SCRIPT
								destruirArchivoOScript(archivoApareado);
								destruirArchivoOScript(
										info->scriptReduccionLocal.nombre);

								//ENVÍO EL ESTADO DE LA EJECUCION DEL SCRIPT REDUCCION LOCAL A MASTER
								resultado_job = malloc(sizeof(int) * 2);
								resultado_job->nodo = id_nodo;
								resultado_job->resultado = estado;
								mensajeAEnviar = serializarResultado(
										resultado_job);
								mensajesEnviadosAMaster(FIN_REDUCCION_LOCAL,
										FDMaster, mensajeAEnviar,
										sizeof(int) * 2);
							}
							break;

						case SOL_REDUCCION_GLOBAL:
							recv(FDMaster, pesoMensaje, 4, 0);
							tamanio = deserializarINT(pesoMensaje);
							mensaje = malloc(tamanio);
							mensaje[tamanio] = '\0';
							if (recv(FDMaster, mensaje, tamanio, 0) == -1) {
								logInfo(
										"Error en la recepcion de Info de Master.");
							} else {
								infoReduccionGlobalDeMasterParaWorker* info =
										deserializarinfoReduccionLocalParaWorker(
												mensaje);
								char* archivoApareado =
										"/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/Worker/tmp/localApareado";

								//REARMO EL SCRIPT
								rearmar_script(&(info->scriptReduccionGlobal),
								REDUCCION_LOCAL);

								//LISTA DE INFO QUE TIENE QUE SOLICITAR A CADA WORKER
								t_list* lista =
										info->listaInfoParaReduccionGlobal;

								//ENVIA	A LOS WORKERS servidores EL PEDIDO DE LOS ARCHIVOS TEMPORALES
								int i = 0;
								cantidadDeWorkersServidoresConectadosAlWorkerElegido =
										list_size(lista);
								while (i
										< cantidadDeWorkersServidoresConectadosAlWorkerElegido) {
									//SE CONECTA A CADA WORKER servidor Y LE SOLICITA EL ARCHIVO TEMPORAL
									infoParaReduccionGlobal* infoReduccionGlobal =
											list_get(lista, i);
									ParametrosComunicacionConWorkerServidor* parametrosWorker =
											setParametrosComunicacionConWorkerServidor(
													infoReduccionGlobal->puerto,
													infoReduccionGlobal->ipWorker);

									pthread_t hiloWorker;
									pthread_create(&hiloWorker, NULL,
											(void*) comunicacionConWorkerServidor,
											parametrosWorker);
									pthread_join(hiloWorker, NULL);

									comunicacionConWorkerServidor(
											infoReduccionGlobal);
									//adentro crea el archivo temporal en worker/tmp/
									//y me carga la lista de archivosTemporalesLocales
								}

								//RECIBE LOS ARCHIVOS TEMPORALES Y LOS GUARDA EN /WORKER/TMP

								char* rutaAGuardarApareo =
										"/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/Worker/tmp/apareoGlobal";
								//HAGO EL APAREO A DICHOS ARCHIVOS
								apareoDeArchivosVectores(
										listaDeArchivosTemporalesLocales,
										rutaAGuardarApareo);

								//EJECUTO EL SCRIPT AL APAREO
								int estadoScriptReductorGlobal =
										ejecutarScriptReductor(
												rutaScriptReduccion,
												rutaAGuardarApareo,
												info->archivoTemporalReduccionGlobal);

								//ENVÍO EL ESTADO DE LA EJECUCION DEL SCRIPT REDUCCION GLOBAL A MASTER
								resultado_job = malloc(sizeof(int) * 2);
								resultado_job->nodo = id_nodo;
								resultado_job->resultado =
										estadoScriptReductorGlobal;
								mensajeAEnviar = serializarResultado(resultado_job);
								int tamanioResultado = sizeof(int) * 2;
								mensajesEnviadosAMaster(FIN_REDUCCION_GLOBAL,	FDMaster, mensajeAEnviar,tamanioResultado);

								//ENVIO EL ARCHIVO TEMPORAL GLOBAL RESULTANTE A FILESYSTEM
								//ARMO LA ESTRUCTURA A ENVIAR
								char* contenido = obtenerPuntero(
										info->archivoTemporalReduccionGlobal);
								char* nombreArchivoReduccionGlobal =
										obtenerSoloNombreDelArchivo(
												info->archivoTemporalReduccionGlobal);

								enviarAlmacenadoFinal(nombreArchivoReduccionGlobal, contenido,
										FD_Filesystem);
							}
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

ParametrosComunicacionConMaster* setParametrosComunicacionConMaster(int puerto) {

	ParametrosComunicacionConMaster* parametros = malloc(
			sizeof(ParametrosComunicacionConMaster));
	parametros->puertoWorker = puerto;
	return parametros;
}

void mensajesEnviadosAMaster(int codigo, int FDMaster, char* mensaje,int tamanio) {
	//Worker recibe de master

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
}

