/*
 * configuracion.c
 *
 *  Created on: 4/9/2017
 *      Author: utnso
 */
#include "Headers/configuracion.h"
#include "commons/config.h"
#include <stdlib.h>
#include <stdio.h>

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

	configuracion->ipYama = strdup(config_get_string_value(configTemporal, YAMA_IP));
	configuracion->puertoYama = config_get_int_value(configTemporal, YAMA_PUERTO);
    configuracion->cantNodos = config_get_int_value(configTemporal, CANT_NODOS);

    config_destroy(configTemporal);

    return configuracion;
}
