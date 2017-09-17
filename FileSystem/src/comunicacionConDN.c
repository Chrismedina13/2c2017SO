/*
 * comunicacionConDN.c
 *
 *  Created on: 16/9/2017
 *      Author: utnso
 */
#include "Headers/comunicacionConYama.h"
#include "Headers/comunicacionConDN.h"

void comunicacionDN(ParametrosComunicacion* parametros){

    logInfo("%i",parametros->puertoFS_dn);
    int FDServidorDN = socketServidor(parametros->puertoFS_dn);
	printf("Se conecto un DataNode su FD es el  = %d\n", FDServidorDN);
	logInfo("FD del DataNode : %i \n", FDServidorDN);


	if (send(FDServidorDN, "Hola DataNode", 13, 0) != -1) {

		puts("Mensaje a DataNode enviado correctamente");

		logInfo("Comunicacion con DataNode establecida");
	} else {
		puts("Error en el envio a Data Node");

		logInfo("Error en la comunicacion con DataNode");
	}
}


