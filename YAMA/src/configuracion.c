/*
 * configuracion.c
 *
 *  Created on: 4/9/2017
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

	configuracion->ipFileSystem = strdup(config_get_string_value(configTemporal, FS_IP));
	configuracion->puertoFileSystem = config_get_int_value(configTemporal, FS_PUERTO);
	configuracion->retardo = config_get_int_value(configTemporal, RETARDO_PLANIFICACION);
	configuracion->algoritmo_bal = strdup(config_get_string_value(configTemporal, ALGORITMO_BALANCEO));
	configuracion->puertoYama = config_get_int_value(configTemporal, YAMA_PUERTO);
	configuracion->disponibilidadBase = config_get_int_value(configTemporal, DISPONIBILIDAD_BASE);
	configuracion->cantNodos = config_get_int_value(configTemporal, CANT_NODOS);

	config_destroy(configTemporal);

    return configuracion;

}

