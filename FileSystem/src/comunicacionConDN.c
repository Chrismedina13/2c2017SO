/*
 * comunicacionConDN.c
 *
 *  Created on: 16/9/2017
 *      Author: utnso
 */
#include "Headers/comunicacionConYama.h"
#include "Headers/comunicacionConDN.h"
#include "SO_lib/estructuras.h"




void comunicacionDN(ParametrosComunicacion* parametros){

	int socketWorkerServidor;
	socketWorkerServidor = lib_socketServidor(parametros->puertoFS_dn);

	fd_set master;
	fd_set read_fds;
	int fd_max;
	int i;
	int FD_Cliente;
	int bytesRecibidos;
	char buffer[20];

	FD_SET(socketWorkerServidor,&master);
	fd_max = socketWorkerServidor;

	for(;;){

		read_fds = master;
		if(select(fd_max+1, &read_fds,NULL,NULL,NULL) == -1){

			logInfo("Error en el select");
		}
		for(i = 0;i <= fd_max; i++){

			if(FD_ISSET(i,&read_fds)){

				if(i == socketWorkerServidor){

					if((FD_Cliente = lib_aceptarYRegistrarSocket(socketWorkerServidor)) == -1){

						logInfo("Error en el aceept despues del select");

					}else{
						FD_SET(FD_Cliente,&master);
						if(FD_Cliente > fd_max){
							fd_max = FD_Cliente;
						}
						logInfo("Nueva conexion del socket cliente DataNode de FD: %i",FD_Cliente); //FD_cliente es mi id_nodo
					//lista ari =	list_create();//aca creo la lisat de nodos y lo meto
					//listadd(i);



					}
				}else{

					//Recibo datos de algun cliente


					if((bytesRecibidos = recv(i,buffer,20,0)) <= 0){

						if(bytesRecibidos == 0){
							logInfo("Conexion cerrada del FD : %i",i);

						}
						close(i);
						FD_CLR(i,&master);

					}else{
						logInfo("Recibi de DATANODE: %s",buffer); // buffer es el mensaje "hola soy data node"
                 // char * nrobloque = malloc(sizeof(int));
                //  int bloque=5;
                // nrobloque = serialize_int(bloque);
                //  int tamanio=strlen(nrobloque);

                 // mensajesEnviadosADataNode(GET_BLOQUE,FD_Cliente,nrobloque,sizeof(tamanio));


						SetBloque *bloque2 = malloc(sizeof(SetBloque));
						bloque2->nrobloque=1;
						bloque2->contenidoBloque="bloque del archivo";

						logInfo("Llego aca");
						char* mensaje= malloc(sizeof(int)+(sizeof(char)*strlen(bloque2->contenidoBloque)));
                        mensaje = serializarBloque(bloque2);
						int tamanioSetBloque= sizeof(int)+(sizeof(char)*strlen(bloque2->contenidoBloque));
						mensajesEnviadosADataNode(SET_BLOQUE, FD_Cliente, mensaje,tamanioSetBloque);


					//free(nrobloque);
						free(mensaje);


					}
				}

				}
			}

		}
}

void mensajesEnviadosADataNode(int codigo, int FD_DataNode, char* mensaje,int tamanio) {
	Paquete * paqueteEnvioDeBloque;
	Paquete* paqueteGetBloque;

	switch (codigo) {

	case SET_BLOQUE:
		paqueteEnvioDeBloque = crearPaquete(SET_BLOQUE, tamanio,mensaje);

		if (enviarPaquete(FD_DataNode, paqueteEnvioDeBloque) == -1) {
			logInfo("Error en envio del Bloque");
		}

		destruirPaquete(paqueteEnvioDeBloque);
		break;

	case GET_BLOQUE:
		logInfo(
				"FILESYSTEM SOLICITA UN BLOQUE");
		paqueteGetBloque= crearPaquete(GET_BLOQUE, tamanio,
				mensaje);

		if (enviarPaquete(FD_DataNode, paqueteGetBloque) == -1) {
			logInfo("Error en envio de GET BLOQUE.");
		}



		destruirPaquete(paqueteGetBloque);

		break;


	default:
		break;
	}
}

void mensajesRecibidosDeDN(int codigo, int FD_DN) {

	char pesoMensaje[8];
	int tamanio;
	char * mensaje;

	switch (codigo) {
	case SET_BLOQUE:

						recv(FD_DN, pesoMensaje, 8, 0);
						tamanio = deserializarINT(pesoMensaje);

						logInfo("tamanio de lo que recibo %i", tamanio);
						//mensaje = malloc(tamanio + 1);
						//mensaje[tamanio] = '\0';
						logInfo("RECIBI OK DE GUARDADO DE BLOQUE POR PARTE DE DATA NODE");



		break;
	case GET_BLOQUE: //RECIBE EL BLOQUE DE DATOS.


						recv(FD_DN,pesoMensaje,8,0);
						tamanio= deserializarINT(pesoMensaje);
						logInfo("tamanio de lo que recibo %i", tamanio);
						logInfo("RECIBO DE DATA NODE CONTENIDO DE UN BLOQUE");
						mensaje = malloc(1024*1024);
				        recv(FD_DN, mensaje,8,0);//chequiar linea
   //no hay que deseralizar por que el contenido es un *char
   //ver como seguir planificando con el bloque recibido
				        free(mensaje);
				        break;
	}
}


/*


//PRUEBA DE MMAP FUNCIONANDO!!!!

	const char* nombreDelArchivoTxt = "/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/FileSystem/archivoprueba.txt";
	const char* nombreDelArchivoBin = "/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/FileSystem/archivoprueba.bin";

 //¡BINARIO!
	int pasoAMemoriaBinario = pasarAMemoriaBinario(nombreDelArchivoBin);
	if(pasoAMemoriaBinario == 0) printf("\nPaso a memoria y division de archivo correcto\n\n");
	else printf("\nError al pasar a memoria\n\n");
//FIN BINARIO

//¡TXT!
	int pasoAMemoriaTxt = pasarAMemoriaTxt(nombreDelArchivoTxt);
	if(pasoAMemoriaTxt == 0) printf("\nPaso a memoria y division de archivo correcto\n\n");
	else printf("\nError al pasar a memoria\n\n");
//FIN TXT.

*/

char *serialize_int(int value){
	size_t size = sizeof(int);
	char *stream = calloc(1, size);
	memcpy(stream, &value, size);
	return stream;
}

