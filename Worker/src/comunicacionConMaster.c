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
					if ((bytesRecibidos = recv(i, buffer,4 , 0)) <= 0) {
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
						int FDMaster = i;
						//ENVIO MASTER
						resultadoJob* resultado_job;
						char* mensajeAEnviar;

						switch (codigo) {
						case TRANSFORMADOR:
							recv(FDMaster, pesoMensaje, 4, 0);
							tamanio = deserializarINT(pesoMensaje);
							mensaje = malloc(tamanio);
							mensaje[tamanio] = '\0';
							if (recv(FDMaster, mensaje, tamanio, 0) == -1) {
								logInfo("Error en la recepcion de Info de Master.");
							}else{
								infoTransformacionParaWorker * info = deserializarInfoParaWorker(mensaje);
								int bloque = info->bloque;
								int bytesOcupados = info->bytesOcupados;
								char* archivoTemporal = info->archivoTemporal;
								//RECIBO EL SCRIPT
								recv(FDMaster, pesoMensaje, 4, 0);
								tamanio = deserializarINT(pesoMensaje);
								mensaje = malloc(tamanio);
								mensaje[tamanio] = '\0';
								if (recv(FDMaster, mensaje, tamanio, 0) == -1) {

									logInfo("Error en la recepcion de Info de Master.");
								}else{

									script* script = deserilizarScript(mensaje);
									rearmar_script(script,SCRIPT_REDUCCION);

									printf("recibo script tranformador");

									//EJECUTO EL SCRIPT EN EL BLOQUE INDICADO
									char* contenido = get_bloque(bloque);
									char* nombreArchivoBloque = "/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/Worker/tmp/archivoBloque";
									crearArchivo(contenido,nombreArchivoBloque);
									int estado = ejecutarScriptTransformador(script->nombre,nombreArchivoBloque,archivoTemporal);//Estado -1 significa que falló


									//BORRO EL ARCHIVO TEMPORAL archivoBloque Y EL SCRIPT
									destruirArchivoOScript(nombreArchivoBloque);
									destruirArchivoOScript(script->nombre);


									//ENVÍO EL ESTADO DE LA EJECUCION DEL SCRIPT TRANSFORMADOR A MASTER
									resultado_job = malloc(sizeof(int)*2);
									resultado_job->nodo = id_nodo;
									resultado_job->resultado = estado;
									mensajeAEnviar = serializarResultado(resultado_job);
									mensajesEnviadosAMaster(FIN_TRANSFORMACION,FD_Cliente,mensajeAEnviar, sizeof(int)*2);
								}
							}
							break;

						case REDUCCION_LOCAL:
							recv(FDMaster, pesoMensaje, 4, 0);
							tamanio = deserializarINT(pesoMensaje);
							mensaje = malloc(tamanio);
							mensaje[tamanio] = '\0';
							if (recv(FDMaster, mensaje, tamanio, 0) == -1) {
								logInfo("Error en la recepcion de Info de Master.");
							}else{
								infoReduccionLocalParaWorker* info = deserializarinfoReduccionLocalParaWorker(mensaje);
								char* archivoApareado = "/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/Worker/tmp/localApareado";

								//REARMO EL SCRIPT
								rearmar_script(&(info->scriptReduccionLocal),REDUCCION_LOCAL);

								//APAREO LOS ARCHIVOS TEMPORALES TRANSFORMADOS QUE VOY A REDUCIR
								apareoDeArchivosVectores(info->listaDeArchivosTemporales,archivoApareado);

								//EJECUTO EL SCRIPT AL SCRIPT
								int estado = ejecutarScriptReductor(info->scriptReduccionLocal.nombre,archivoApareado,info->archivoTemporalReduccionLocal);

								//BORRO EL ARCHIVO TEMPORAL archivoBloque Y EL SCRIPT
								destruirArchivoOScript(archivoApareado);
								destruirArchivoOScript(info->scriptReduccionLocal.nombre);

								//ENVÍO EL ESTADO DE LA EJECUCION DEL SCRIPT REDUCCION LOCAL A MASTER
								resultado_job = malloc(sizeof(int)*2);
								resultado_job->nodo = id_nodo;
								resultado_job->resultado = estado;
								mensajeAEnviar = serializarResultado(resultado_job);
								mensajesEnviadosAMaster(FIN_REDUCCION_LOCAL,FD_Cliente,mensajeAEnviar, sizeof(int)*2);
							}
							break;

						case REDUCCION_GLOBAL:
							recv(FDMaster, pesoMensaje, 4, 0);
							tamanio = deserializarINT(pesoMensaje);
							mensaje = malloc(tamanio);
							mensaje[tamanio] = '\0';
							if (recv(FDMaster, mensaje, tamanio, 0) == -1) {
								logInfo("Error en la recepcion de Info de Master.");
							}else{
								infoReduccionGlobalDeMasterParaWorker* info = deserializarinfoReduccionLocalParaWorker(mensaje);
								char* archivoApareado = "/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/Worker/tmp/localApareado";

								//REARMO EL SCRIPT
								rearmar_script(&(info->scriptReduccionGlobal),REDUCCION_LOCAL);

								//LISTA DE INFO QUE TIENE QUE SOLICITAR A CADA WORKER
								t_list* lista = info->listaInfoParaReduccionGlobal;

								//ENVIA	A LOS WORKERS servidores EL PEDIDO DE LOS ARCHIVOS TEMPORALES
								int i = 0;
								char* rutaAGuardar = "/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/Worker/tmp/";
								while(i<list_size(lista)){
									//SE CONECTA A CADA WORKER servidor Y LE SOLICITA EL ARCHIVO TEMPORAL
									infoParaReduccionGlobal* infoReduccionGlobal = list_get(lista,i);
									comunicacionConWorkerServidor(infoReduccionGlobal);

								}

								//RECIBE LOS ARCHIVOS TEMPORALES Y LOS GUARDA EN /WORKER/TMP (se podria hacer un semaforo esperando
								// a que todos los workers manden sus archivos temporales).


								//HAGO EL APAREO A DICHOS ARCHIVOS


								//EJECUTO EL SCRIPT AL APAREO


								//ENVIO EL ESTADO DE LA EJECUCION DEL SCRIPT A MASTER

								//ENVIO EL ARCHIVO TEMPORAL GLOBAL RESULTANTE A FILESYSTEM


								//ENVÍO EL ESTADO DE LA EJECUCION DEL SCRIPT REDUCCION GLOBAL A MASTER
/*								resultado_job = malloc(sizeof(int)*2);
								resultado_job->nodo = id_nodo;
								resultado_job->resultado = estado;
								mensajeAEnviar = serializarResultado(resultado_job);
								mensajesEnviadosAMaster(FIN_REDUCCION_LOCAL,FD_Cliente,mensajeAEnviar, sizeof(int)*2);

*/							}
							break;
		//				case SOL_ARCHIVOS_TEMPORALES_LOCALES: //WORKER ELEGIDO A WORKER
							//ACA VA LO QUE PASA CUANDO EL WORKER ELEGIDO SE CONECTA A OTRO WORKER

							//ENVIO EL ARCHIVO AL WORKER ELEGIDO EL ARCHIVO QUE ME HAYA SOLICITADO
		//					break;
						}
		//				case RECEPCION_ARCHIVO_TEMPORAL_LOCAL:
							//ACA VA LO QUE PASA CUANDO UNO DE LOS WORKERS LE ENVIA AL WORKER ELEGIDO SU ARCHIVO TEMPORAL


							/*En vez de semaforos aca podriamos hacer un if que verifique cuantos workes ennviaron su
							 * archivo temporal con una variable global que vaya aumentando cada vez que entra en este
							 * case. Y solo entra cuando la variable global sea igual a la cantidad de workers que
							 * tienen que enviarle un archivo temporal local */
			//				break;




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



void mensajesEnviadosAMaster(int codigo, int FDMaster, char* mensaje, int tamanio) {
	//Worker recibe de master
	char pesoMensaje[4];


	switch (codigo) {
	case FIN_TRANSFORMACION:
		logInfo(
				"Woker envia señal de finalización de Transformación(EXITO o FRACASO)");
		Paquete* paqueteTranf = crearPaquete(FIN_TRANSFORMACION, tamanio, mensaje);

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
