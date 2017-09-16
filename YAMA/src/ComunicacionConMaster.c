/*
 * ComunicacionConMaster.c
 *
 *  Created on: 16/9/2017
 *      Author: utnso
 */
#include "Headers/ComunicacionConMaster.h"

void comunicacionConMaster(ParametrosComunicacionConMaster* parametro) {
	//socketServer
	int FDServidor = socketServidor(parametro->puerto);
	logInfo("Se conecto un Master su FD es el  = %d\n", FDServidor);
	if (send(FDServidor, "Soy YAMA", 8, 0) != -1) {
		logInfo("Mensaje a Master enviado correctamente");
	} else {
		logInfo("Error en el envio");
	}
}

ParametrosComunicacionConMaster* setParametrosComunicacionConMaster(int puerto){
	ParametrosComunicacionConMaster* parametros = malloc(sizeof(ParametrosComunicacionConMaster));
	parametros->puerto = puerto;
	return parametros;
}



