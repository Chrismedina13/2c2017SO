#include "Headers/ComunicacionConWorker.h"
#include "SO_lib/estructuras.h"

void comunicacionWorkers(ParametrosComunicacionWoker* parametros) {
	int FDServidorWORKER;
	FDServidorWORKER = lib_SocketCliente(parametros->ipWoker,parametros->puertoWoker);

		printf("Se conecto un Worker su FD es el  = %d\n", FDServidorWORKER);

		logInfo("FD del Worker : %i \n", FDServidorWORKER);

		if (send(FDServidorWORKER, "Hola WORKER", 13, 0) != -1) {


				logInfo("Comunicacion con WORKER establecida, Mensaje enviado correctamente");
			} else {

				logInfo("Error en la comunicacion con WORKER ");

			}

}
ParametrosComunicacionWoker* setParametrosComunicacionConWoker(int puerto, char* ip) {

	ParametrosComunicacionWoker* parametros = malloc(
			sizeof(ParametrosComunicacionWoker));
	parametros->ipWoker = ip;
	parametros->puertoWoker = puerto;
	return parametros;
}

