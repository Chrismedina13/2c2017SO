/*
 * comunicacionConWorker.c
 *
 *  Created on: 19/9/2017
 *      Author: utnso
 */

#include "Headers/comunicacionConYama.h"
#include "Headers/comunicacionConDN.h"
#include "Headers/comunicacionConWorker.h"


void comunicacionWorker(ParametrosComunicacion* parametros){

	int FDServidorWORKER = socketServidor(parametros->puertoFS_worker);
		printf("Se conecto un Yama su FD es el  = %d\n", FDServidorWORKER);
	    logInfo("FD del Yama : %i \n", FDServidorWORKER);

		if (send(FDServidorWORKER, "Hola WORKER", 13, 0) != -1) {


				logInfo("Comunicacion con WORKER establecida, Mensaje enviado correctamente");
			} else {

				logInfo("Error en la comunicacion con WORKER ");

			}
}
