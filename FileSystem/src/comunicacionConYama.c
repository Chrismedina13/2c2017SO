/*
 * comunicacionConYama.c
 *
 *  Created on: 16/9/2017
 *      Author: utnso
 */
#include "Headers/comunicacionConYama.h"
#include "Headers/comunicacionConDN.h"
#include "Headers/FileSystem.h"
#include "SO_lib/sem.h"


void comunicacionYAMA(ParametrosComunicacion* parametros) {

	//semaphore_wait(SEMAFOROYAMA);


	char codigoRecibido[4];
	int codigo;

	int FDServidorYAMA = socketServidor(parametros->puertoFS_yama);
	printf("Se conecto un Yama su FD es el  = %d\n", FDServidorYAMA);
	logInfo("FD del Yama : %i \n", FDServidorYAMA);

	if (send(FDServidorYAMA, "Hola YAMA", 13, 0) != -1) {

		logInfo("Comunicacion con YAMA establecida, Mensaje enviado correctamente");
	} else {

		logInfo("Error en la comunicacion con YAMA ");

	}

	logInfo("Creando mensaje de ubicacion de nodos");

	char* ipWorker1 = "127.0.0.1";
	int puerto1 = 5050;
	Info_Workers* info1 = malloc(sizeof(ipWorker1)+ sizeof(int));
	char* ipWorker2 = "127.0.0.1";
	int puerto2 = 5050;
	Info_Workers* info2 = malloc(sizeof(ipWorker2)+ sizeof(int));
	char* ipWorker3 = "127.0.0.1";
	int puerto3 = 5050;
	Info_Workers* info3 = malloc(sizeof(ipWorker3)+ sizeof(int));

	info1->ipWorker = ipWorker1;
	info1->puerto = puerto1;
	info2->ipWorker = ipWorker2;
	info2->puerto = puerto2;
	info3->ipWorker = ipWorker3;
	info3->puerto = puerto3;

	t_list* LISTARRG = list_create();
	list_add(LISTARRG,info1);
	list_add(LISTARRG,info2);
	list_add(LISTARRG,info3);

	int tamanioInfoWorkerAEnviar = (tamanioEstructurasListaWorkers(LISTARRG) + ((sizeof(int)* list_size(LISTARRG))*2) + sizeof(int));
	char* listaSerializada = serializarLista_info_workers(LISTARRG);
	mensajesEnviadosAYama(INFO_WORKER,FDServidorYAMA,listaSerializada,tamanioInfoWorkerAEnviar);

/*    logInfo("creando Respuesta Almacenado Final de prueba ");

    respuestaAlmacenadoFinal* RAF = crearRespuestaAlmacenadoFinal(5,200,"20.121.21.33","hola.txt");

    int tamanioRAFaMandar = sizeof(int)*5 + strlen(RAF->archivoDeReduccionGlobal) + strlen(RAF->ipWorker);

    char* rafSerializada = serializarRespuestaAlmacenadoFinal(RAF);

    mensajesEnviadosAYama(PRUEBAS,FDServidorYAMA,rafSerializada,tamanioRAFaMandar);

    logInfo("Respuesta Almacenado Final de prueba listo para enviar ");

	logInfo("Creando info reduccion local para worker");
    t_list* listaArchivosTemporales = list_create();
    char* archivo0 = "archivoTemporal0.txt";
    char* archivo1 = "archivoTemporal1.txt";
    char* archivo2 = "archivoTemporal2.txt";
    char* archivo3 = "archivoTemporal3.txt";
    list_add(listaArchivosTemporales,archivo0);
    list_add(listaArchivosTemporales,archivo1);
    list_add(listaArchivosTemporales,archivo2);
    list_add(listaArchivosTemporales,archivo3);

    char* contenido = "Soy un script creado para la prueba, ojala llegue bien a yama para probar que mando bien la estructura reduccion local para worker";
    char* nombre = "HOLA soy un script";

    script scriptAMandar;
    scriptAMandar.nombre = nombre;
    scriptAMandar.contenido = contenido;


    infoReduccionLocalParaWorker* info = crearInfoReduccionLocalParaWorker(listaArchivosTemporales,"archivoDeReduccionLocal",scriptAMandar);

	int tamanioAMandar = tamanioinfoReduccionLocalParaWorker(info) + (sizeof(int) * list_size(info->listaDeArchivosTemporales)) + (sizeof(int)*5);

    char* infoSerializado = serializarinfoReduccionLocalParaWorker(info);

    mensajesEnviadosAYama(PRUEBAS,FDServidorYAMA,infoSerializado,tamanioAMandar);

    logInfo("Mandando lista serializada de prueba");

    logInfo(" Armando lista ubicaciones");

    t_list* lista_ubicaciones = list_create();
	UbicacionBloquesArchivo2 * ubicaciones1 = crearUbicacionBloquesArchivos2(0,1,12,23,34,45);
	list_add(lista_ubicaciones, ubicaciones1);
	UbicacionBloquesArchivo2 * ubicaciones2 = crearUbicacionBloquesArchivos2(2,50,60,45,322,125454);
	list_add(lista_ubicaciones, ubicaciones2);
	UbicacionBloquesArchivo2 * ubicaciones3 = crearUbicacionBloquesArchivos2(3,4545,02323,3434,33434,5555555);
	list_add(lista_ubicaciones, ubicaciones3);

    logInfo("Lista ubicaciones armada");

	char* lista_serializada=serializarListaUbicacionBloquesArchivos(lista_ubicaciones);
	int tamanio_lista_serializada= (24*list_size(lista_ubicaciones)) + sizeof(int)+ (sizeof(int)*list_size(lista_ubicaciones)) ;

    logInfo(" todo serializado listo para mandar");

    mensajesEnviadosAYama(UBICACION_BLOQUES,FDServidorYAMA,lista_serializada,tamanio_lista_serializada);

    logInfo("Se va a enviar a YAMA las ubicaciones ");




    //cm:recibe de yama el nombre del archivo
	char buffer[4];
	recv(FDServidorYAMA,buffer,4,0);
	codigo = deserializarINT(buffer);
	logInfo("Recibi de Yama: %i", codigo);
	mensajesRecibidosDeYama(codigo, FDServidorYAMA);



*/

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



	t_list * lista_ubicaciones = list_create();
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


      lista_ubicaciones = nombreToUbicaciones(mensaje);
	  //  logInfo(" Armando lista ubicaciones");


	//	UbicacionBloquesArchivo2 * ubicaciones1 = crearUbicacionBloquesArchivos2(0,2,21,21,32,12);
	//	list_add(lista_ubicaciones, ubicaciones1);
	//	UbicacionBloquesArchivo2 * ubicaciones2 = crearUbicacionBloquesArchivos2(0,2,21,21,32,12);
	//	list_add(lista_ubicaciones, ubicaciones2);
	//	UbicacionBloquesArchivo2 * ubicaciones3 = crearUbicacionBloquesArchivos2(0,2,21,21,32,12);
	//	list_add(lista_ubicaciones, ubicaciones3);

	//    logInfo("Lista ubicaciones armada");

		lista_serializada=malloc(sizeof(UbicacionBloquesArchivo)*list_size(lista_ubicaciones));
		lista_serializada=serializarListaUbicacionBloquesArchivo2(lista_ubicaciones);
		tamanio_lista_serializada= sizeof(UbicacionBloquesArchivo)*list_size(lista_ubicaciones);

	    logInfo(" todo serializado listo para mandar");

	    mensajesEnviadosAYama(UBICACION_BLOQUES,FDYama,lista_serializada,tamanio_lista_serializada);

	    logInfo("Se va a enviar a YAMA las ubicaciones ");
		break;



	default:
		break;
	}
}



void mensajesEnviadosAYama(int codigo,int FD_YAMA, char* mensaje,int tamanio){
	Paquete* paqueteEnvioUbicacionBloque;

	switch (codigo) {
		case PRUEBAS:

			paqueteEnvioUbicacionBloque = crearPaquete(codigo, tamanio,mensaje);

			if (enviarPaquete(FD_YAMA, paqueteEnvioUbicacionBloque) == -1) {
				logInfo("Error en envio de la prueba RAF");
				}else{

				logInfo("Enviado correctamente la prueba");
								}
			    destruirPaquete(paqueteEnvioUbicacionBloque);

			break;
		case UBICACION_BLOQUES:

			paqueteEnvioUbicacionBloque = crearPaquete(codigo, tamanio,mensaje);

					if (enviarPaquete(FD_YAMA, paqueteEnvioUbicacionBloque) == -1) {
						logInfo("Error en envio de la ubicacion de los bloques");
					}else{

						logInfo("Enviado correctamente");
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


UbicacionBloquesArchivo * setearUbicacionBloque(int nodo1, int bloquenodo1, int nodo2, int bloquenodo2, int parteDelArchivo,int bytesOcupados){
	UbicacionBloquesArchivo * ubicaciones;
	ubicaciones->bytesOcupados= bytesOcupados;
	ubicaciones->parteDelArchivo=parteDelArchivo;
	ubicaciones->ubicacionCopia1.desplazamiento=bloquenodo1;
    ubicaciones->ubicacionCopia1.nodo=nodo1;
    ubicaciones->ubicacionCopia2.desplazamiento=bloquenodo2;
    ubicaciones->ubicacionCopia2.nodo=nodo2;
    return ubicaciones;
}






