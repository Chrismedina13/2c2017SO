#include "Headers/ComunicacionConYama.h"

void comunicacionYama(ParametrosComunicacionYAMA* parametros) {

	logInfo("%i,%s",parametros->puertoYAMA,parametros->ipYAMA);

	int FDsocketClienteYAMA;
	FDsocketClienteYAMA = lib_SocketCliente(parametros->ipYAMA, parametros->puertoYAMA);

	logInfo("SocketCliente YAMA = %d \n", FDsocketClienteYAMA);

	if(send(FDsocketClienteYAMA,"Hola Yama soy Master", 20, 0) != -1){

		logInfo("Mensaje a Yama mandado correctamente");

	}else{

		logInfo("Error envio de mensaje a YAMA");

	}

}

ParametrosComunicacionYAMA* setParametrosComunicacionYAMA(int puerto,char* ip){

	ParametrosComunicacionYAMA* parametros = malloc(sizeof(ParametrosComunicacionYAMA));
	parametros->ipYAMA = ip;
	parametros->puertoYAMA = puerto;
	return parametros;
}
