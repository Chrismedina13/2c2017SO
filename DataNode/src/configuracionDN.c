/*
 * configuracion.c
 *
 *  Created on: 7/9/2017
 *      Author: utnso
 */
#include "Headers/configuracionDN.h"


Configuracion* leerArchivoDeConfiguracion(char* pathArchivoDeConfiguracion){

	Configuracion* configuracion = malloc(sizeof(Configuracion));
	t_config* configTemporal;

	if(pathArchivoDeConfiguracion == NULL){
		puts("No se pudo leer archivo de configuracion o no existe la ruta");
	}
	else{
		configTemporal = config_create(pathArchivoDeConfiguracion);

		if(configTemporal == NULL){

		puts("No se pudo cargar archivo de configuracion");
		exit(1);
		}
	}

	configuracion->ipFileSystem = strdup(config_get_string_value(configTemporal, IP_FILESYSTEM));
	configuracion->puertoFileSystem = config_get_int_value(configTemporal, PUERTO_FILESYSTEM1);
	configuracion->nombreNodo = strdup(config_get_string_value(configTemporal, NOMBRE_NODO));
    configuracion->puertoWorker = config_get_int_value(configTemporal,PUERTO_WORKER);
    configuracion->puertoDataNode = config_get_int_value(configTemporal,PUERTO_DATANODE);
    configuracion->rutaDataBin = strdup(config_get_string_value(configTemporal,RUTA_DATABIN));

    config_destroy(configTemporal);

    return configuracion;

}


