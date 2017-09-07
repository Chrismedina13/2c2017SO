/*
 * configuracion.c
 *
 *  Created on: 7/9/2017
 *      Author: utnso
 */
#include "Headers/configuracion.h"

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
	configuracion->puerto = config_get_int_value(configTemporal, PUERTO);

    config_destroy(configTemporal);

    return configuracion;

}