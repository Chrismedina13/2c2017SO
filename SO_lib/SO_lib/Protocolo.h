/*
 * Protocolo.h
 *
 *  Created on: 29/9/2017
 *      Author: utnso
 */

#ifndef SO_LIB_PROTOCOLO_H_
#define SO_LIB_PROTOCOLO_H_

//YAMA - MASTER
#define NOMBRE_ARCHIVO 1
#define SOL_TRANSFORMACION 2
#define FIN_TRANSFORMACION 3
#define SOL_REDUCCION_LOCAL 4
#define FIN_REDUCCION_LOCAL 5
#define SOL_REDUCCION_GLOBAL 6
#define FIN_REDUCCION_GLOBAL 7
#define SOL_ALMACENADO_FINAL 8
#define REPLANIFICACION 9
#define FINALIZACION_DE_JOB 10
#define SCRIPT_REDUCCION 20
#define SCRIPT_TRANSFORMADOR 21
#define SCRIPT_TRANSFORMADOR_INICIAL 22
#define SCRIPT_TRANSFORMADOR_ANUAL 23



//MASTER - FS
#define ALMACENADO_FINAL 9

//FS - DATA NODE
#define GET_BLOQUE 12
#define SET_BLOQUE 10
#define IP_NODO 14
#define SALUDO 15
#define RTA_GET_BLOQUE 18
#define RTA_SET_BLOQUE 19



//FS - YAMA
#define UBICACION_BLOQUES 11
#define INFO_WORKER 13

//MASTER - WORKER
#define TAREA_WORKER 16
#define REDUCCION_TEMPORALES 17

#endif /* SO_LIB_PROTOCOLO_H_ */
