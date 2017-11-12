/*
 * comunicacionConYama.c
 *
 *  Created on: 16/9/2017
 *      Author: utnso
 */
#include "Headers/comunicacionConYama.h"
#include "Headers/comunicacionConDN.h"


void comunicacionYAMA(ParametrosComunicacion* parametros) {

	int FDServidorYAMA = socketServidor(parametros->puertoFS_yama);
	printf("Se conecto un Yama su FD es el  = %d\n", FDServidorYAMA);
	logInfo("FD del Yama : %i \n", FDServidorYAMA);

	if (send(FDServidorYAMA, "Hola YAMA", 13, 0) != -1) {

		logInfo(
				"Comunicacion con YAMA establecida, Mensaje enviado correctamente");
	} else {

		logInfo("Error en la comunicacion con YAMA ");

	}

	//fs manda la lista de workers
	char * lista_workers = malloc((11+sizeof(char) + sizeof(int))*list_size(list_info_workers));
	       lista_workers=serializarLista_info_workers(list_info_workers);
		   int tamanio_lista_workers = (11+sizeof(char) + sizeof(int))*list_size(list_info_workers);
	mensajesEnviadosAYama(INFO_WORKER, FDServidorYAMA, lista_workers, tamanio_lista_workers);


	//cm:recibe de yama el nombre del archivo
	char buffer[4];
	recv(FDServidorYAMA,buffer,4,0);
	int codigo = deserializarINT(buffer);
	logInfo("Recibi de Yama: %i", codigo);
	mensajesRecibidosDeYama(codigo, FDServidorYAMA);





}


ParametrosComunicacion* setParametrosComunicacion(int puertoDN, int puertoYAMA,
		int puertoWorker) {

	ParametrosComunicacion* parametros = malloc(sizeof(ParametrosComunicacion));
	parametros->puertoFS_dn = puertoDN;
	parametros->puertoFS_yama = puertoYAMA;
	parametros->puertoFS_worker = puertoWorker;
	return parametros;
}
void mensajesRecibidosDeYama(int codigo, int FDYama) {

	char pesoMensaje[4];
	int tamanio;
	char* mensaje;
	char pesoMensaje2[8];

	t_list* ubicacionBloques;
	t_list * lista_ubicaciones;
	char* lista_serializada;
	int tamanio_lista_serializada;


	switch (codigo) {
	case NOMBRE_ARCHIVO:
		recv(FDYama, pesoMensaje, 4, 0);
		tamanio = deserializarINT(pesoMensaje);
		logInfo("tamanio de lo que recibo %i", tamanio);
		mensaje = malloc(tamanio + 1);
		mensaje[tamanio] = '\0';
		recv(FDYama, mensaje, tamanio, 0);
		logInfo("Se recibio el nombre del archivo: %s de tamanio %i", mensaje,
				tamanio);


         	//ubicacionBloques = nombreToUbicaciones(mensaje);
			lista_serializada=malloc(sizeof(UbicacionBloquesArchivo)*list_size(ubicacionBloques));
			lista_serializada=serializarListaUbicacionBloquesArchivos(ubicacionBloques);
			tamanio_lista_serializada= sizeof(UbicacionBloquesArchivo)*list_size(ubicacionBloques);
			mensajesEnviadosAYama(UBICACION_BLOQUES,FDYama,ubicacionBloques,tamanio_lista_serializada);

		break;



	default:
		break;
	}
}



void mensajesEnviadosAYama(int codigo,int FD_YAMA, char* mensaje,int tamanio){
	Paquete* paqueteEnvioUbicacionBloque;

	switch (codigo) {
		case UBICACION_BLOQUES:

			paqueteEnvioUbicacionBloque = crearPaquete(codigo, tamanio,mensaje);

					if (enviarPaquete(FD_YAMA, paqueteEnvioUbicacionBloque) == -1) {
						logInfo("Error en envio de la ubicacion de los bloques");
					}

					destruirPaquete(paqueteEnvioUbicacionBloque);

			break;
		case INFO_WORKER:
			paqueteEnvioUbicacionBloque = crearPaquete(codigo, tamanio,mensaje);

			if (enviarPaquete(FD_YAMA, paqueteEnvioUbicacionBloque) == -1) {
				logInfo("Error en envio de la ubicacion de los bloques");
			}

			destruirPaquete(paqueteEnvioUbicacionBloque);

	break;

		default:
			break;
	}
}







