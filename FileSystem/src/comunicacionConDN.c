/*
 * comunicacionConDN.c
 *
 *  Created on: 16/9/2017
 *      Author: utnso
 */
#include "Headers/comunicacionConYama.h"
#include "Headers/comunicacionConDN.h"



void comunicacionDN(ParametrosComunicacion* parametros){

	int socketWorkerServidor;
	socketWorkerServidor = lib_socketServidor(parametros->puertoFS_dn);

	fd_set master;
	fd_set read_fds;
	int fd_max;
	int i;
	int FD_Cliente;
	int bytesRecibidos;
	char buffer[5];

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
					//aca creo la lisat de nodos y lo meto

					}
				}else{

					//Recibo datos de algun cliente


					if((bytesRecibidos = recv(i,buffer,4,0)) <= 0){

						if(bytesRecibidos == 0){
							logInfo("Conexion cerrada del FD : %i",i);

						}
						close(i);
						FD_CLR(i,&master);

					}else{
						int codigo = deserializarINT(buffer);
						//mensajesRecibidosDeDN(codigo,i);

						logInfo("Recibi de DATANODE: %s",buffer); // buffer es el mensaje
					}
				}

				}
			}

		}
}

/*void mensajesEnviadosADataNode(int codigo, int FD_DataNode, char* mensaje,int tamanio) {
	switch (codigo) {
	case SET_BLOQUE:
		Paquete* paqueteEnvioDeBloque = crearPaquete(SET_BLOQUE, tamanio,mensaje);

		if (enviarPaquete(FD_DataNode, paqueteEnvioDeBloque) == -1) {
			logInfo("Error en envio del Bloque");
		}

		destruirPaquete(paqueteEnvioDeBloque);
		break;

	case GET_BLOQUE:
		logInfo(
				"FILESYSTEM SOLICITA UN BLOQUE");
		Paquete* paqueteGetBloque= crearPaquete(GET_BLOQUE, tamanio,
				mensaje);

		if (enviarPaquete(FD_DataNode, paqueteGetBloque) == -1) {
			logInfo("Error en envio de respuesta de Transformacion.");
		}

		destruirPaquete(paqueteGetBloque);

		break;


	default:
		break;
	}
}

void mensajesRecibidosDeDN(int codigo, int FD_DN) {
	switch (codigo) {
	case SET_BLOQUE:
	//	char* buffer = malloc(4);


		//recv(FD_DN,buffer,4,0);
		//int tamanio= deserializarINT(buffer);
		//char* codigo = malloc(tamanio);
		//recv(FD_DN,codigo, tamanio);





		//PLANIFICA
		logInfo("Master recibe de Yama solicitud de transformación.");



		break;
	case GET_BLOQUE:
		logInfo("Master recibe de Yama solicitud de Reducción Local.");

		break;

	}
}

//PRUEBA DE MMAP FUNCIONANDO!!!!
/*

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


