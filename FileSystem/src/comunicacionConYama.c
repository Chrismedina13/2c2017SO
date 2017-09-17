/*
 * comunicacionConYama.c
 *
 *  Created on: 16/9/2017
 *      Author: utnso
 */
#include "Headers/comunicacionConYama.h"
#include "Headers/comunicacionConDN.h"

void comunicacionYAMA(ParametrosComunicacion* parametros){

	int FDServidorYAMA = socketServidor(parametros->puertoFS_yama);
		printf("Se conecto un Yama su FD es el  = %d\n", FDServidorYAMA);
	    logInfo("FD del Yama : %i \n", FDServidorYAMA);

		if (send(FDServidorYAMA, "Hola YAMA", 13, 0) != -1) {


				logInfo("Comunicacion con YAMA establecida, Mensaje enviado correctamente");
			} else {

				logInfo("Error en la comunicacion con YAMA ");

			}
}

ParametrosComunicacion* setParametrosComunicacion(int puertoDN,int puertoYAMA){

	ParametrosComunicacion* parametros = malloc(sizeof(ParametrosComunicacion));
	parametros->puertoFS_dn = puertoDN;
	parametros->puertoFS_yama = puertoYAMA;
	return parametros;
}
