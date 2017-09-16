#include "Headers/ComunicacionConYama.h"

void comunicacionYama(ParametrosComunicacionYAMA* parametros) {

	logInfo("%i,%s",parametros->puertoYAMA,parametros->ipYAMA);

	int FDsocketClienteYAMA;
	FDsocketClienteYAMA = SocketCliente(parametros->ipYAMA, parametros->puertoYAMA);

	logInfo("SocketCliente YAMA = %d \n", FDsocketClienteYAMA);

	char buffer2[8];
	recv(FDsocketClienteYAMA, buffer2, 8, 0);
	logInfo("Se recibio de YAMA: %s", buffer2);

}

ParametrosComunicacionYAMA* setParametrosComunicacionYAMA(int puerto,char* ip){

	ParametrosComunicacionYAMA* parametros = malloc(sizeof(ParametrosComunicacionYAMA));
	parametros->ipYAMA = ip;
	parametros->puertoYAMA = puerto;
	return parametros;
}
