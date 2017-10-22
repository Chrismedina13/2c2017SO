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

//MASTER - FS
#define ALMACENADO_FINAL 9

//FS - DATA NODE
#define GET_BLOQUE 12
#define SET_BLOQUE 13
#define ENVIO_BLOQUE 10



//FS - YAMA
#define UBICACION_BLOQUES 11



#endif /* SO_LIB_PROTOCOLO_H_ */
