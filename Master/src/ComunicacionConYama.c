#include "Headers/comunicacionConYama.h"

void comunicacionYama(int puertoYama, int ipYama) {

	int FDsocketClienteYAMA;
	FDsocketClienteYAMA = SocketCliente(ipYama, puertoYama);

	logInfo("SocketCliente YAMA = %d \n", FDsocketClienteYAMA);

	char buffer2[8];
	recv(FDsocketClienteYAMA, buffer2, 8, 0);
	logInfo("Se recibio: %s", buffer2);

}
