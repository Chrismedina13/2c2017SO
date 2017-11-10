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

	char* ubicacionBloques;

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
		//recorro la lista de archivos, y mando la siguiente estructura:

	ubicacionBloques =setearUbicacionBloqueYSerealizar(1,12, 2, 4, 3,1024);// hardcodeo
	mensajesEnviadosAYama(UBICACION_BLOQUES,FDYama,ubicacionBloques,strlen(ubicacionBloques) );

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
		default:
			break;
	}
}

char* setearUbicacionBloqueYSerealizar(int nodo1, int bloquenodo1, int nodo2, int bloquenodo2, int parteDelArchivo,int bytesOcupados){

          char* ubicacion=malloc(sizeof(int)*6);

	UbicacionBloque ubicacionbloque1;
	ubicacionbloque1.nodo=nodo1;
	ubicacionbloque1.bloqueDelNodoDeLaCopia= bloquenodo1;
	UbicacionBloque ubicacionbloque2;
	ubicacionbloque2.nodo=nodo2;
	ubicacionbloque2.bloqueDelNodoDeLaCopia=bloquenodo2;
	UbicacionBloquesArchivo * ubicacionbloquesarchivo;
	ubicacionbloquesarchivo->parteDelArchivo=parteDelArchivo;
	ubicacionbloquesarchivo->bytesOcupados=bytesOcupados;
	ubicacionbloquesarchivo->ubicacionCopia1=ubicacionbloque1;
	ubicacionbloquesarchivo->ubicacionCopia1=ubicacionbloque2;

	ubicacion = serializarUblicacionBloqueArchivo(ubicacionbloquesarchivo);


	return(&ubicacion);



}

