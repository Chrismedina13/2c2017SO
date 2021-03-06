/*
 * estructuras.h
 *
 *  Created on: 22/9/2017
 *      Author: utnso
 */

#ifndef SO_LIB_ESTRUCTURAS_H_
#define SO_LIB_ESTRUCTURAS_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "commons/collections/list.h"
#include "sockets.h"
//YAMA armara una lista con esta estructura , la serializara y mandara a Master que deserealizara

//int DISTRIBUCION_NODOS=0;
typedef struct ParametroParaWorker{
 	int nodo;
 	int puerto;
 	char* ip;
 	t_list* transformaciones; //lista de estructura tareaTransformacion;
 }ParametroParaWorker;

 typedef struct tareaTransformacion{
 	int bloque;
 	int bytesOcupados;
 	char* archivoTemporal;
 }tareaTransformacion;

typedef struct resultadoJob{

	int nodo;
	int resultado;

}resultadoJob;


typedef struct finTransformacion{

	int nodo;
	int numeroDeJob;
	int bloque;
	int resultado;

}finTransformacion; //Tambien la uso Para Fin reduccion Global me mandan el numero de job y el nodo encargado

typedef struct finReduccionLocal{
	int nodo;
	int numeroDeJob;
	int resultado;
}finReduccionLocal;

typedef struct job{

	int master; //FD del master que me mando el job
	char* nombreDelArchivo; //nombre del archivo del job
	int identificadorJob; //numero que se le asigno al job
}Job;

typedef struct jobCompleto{

	Job* job; //Puntero a una estructura JOB
	t_list* ubicacionDeLasPartesDelJOB;
	t_list* respuestaTransformacion;
	int nodoEncargado;
}JOBCompleto;


typedef struct RespuestaTransformacionYAMA {
	int nodo;
	int puertoWorker;
	char* ipWorkwer;
	int bloque;
	int bytesOcupados;
	char* archivoTemporal;

}__attribute__((packed))RespuestaTransformacionYAMA;

typedef struct respuestaReduccionLocal{

	int nodo;
	int puertoWorker;
	char* ipWorker;
	t_list* archivosDeTransformacion;
	char* archivoReduccionLocal;
}RespuestaReduccionLocal;

typedef struct respuestaReduccionGlobal{

	int nodo;
	int puertoWorker;
	char* ipWorker;
	char* archivoReduccionLocal;
	char* archivoReduccionGlobal;
	bool encargado;
}RespuestaReduccionGlobal;


typedef struct repuestaAlmacenadoFinal{

	int nodo;
	int puertoWorker;
	char* ipWorker;
	char* archivoDeReduccionGlobal;

}respuestaAlmacenadoFinal;


//paquete para mandar y recibir mensajes
typedef struct Paquete {
	uint32_t codigo;
	uint32_t tamanio;
	char* mensaje;
} Paquete;
//nodoParaPlanificar es un nodo de la lista de listaDeWorkerTotales
typedef struct nodo {

	int nodo;
	int disponibilidad;
	int carga;
	t_list* partesDelArchivo;
	t_list* partesAplanificar;
} nodoParaPlanificar;

typedef struct ubicacionBloque {
	int nodo;  //Nodo donde se encuentra la copia
	int desplazamiento; // Bloque adentro del nodo donde se encuentra. Desplazamiento
} UbicacionBloque;

typedef struct ubiacionBloquesArchivo {
	int parteDelArchivo; //numero del bloque del archivo
	UbicacionBloque ubicacionCopia1; //ubicacion copia 1
	UbicacionBloque ubicacionCopia2; //ubicacion copia 2
	int bytesOcupados; //bytesOCupados por ese bloque
} UbicacionBloquesArchivo;

typedef struct ubicacionBloquesArchivo2{
	int parteDelArchivo; //numero del bloque del archivo
	int nodo1;  //Nodo donde se encuentra la copia
	int desplazamiento1; // Bloque adentro del nodo donde se encuentra. Desplazamiento
	int nodo2;  //Nodo donde se encuentra la copia
	int desplazamiento2; // Bloque adentro del nodo donde se encuentra. Desplazamiento
	int bytesOcupados; //bytesOCupados por ese bloque
} UbicacionBloquesArchivo2;

typedef struct SetBloque{
	int nrobloque;
	char* contenidoBloque;
}SetBloque;

typedef struct Info_Workers{
	int puerto;
	char* ipWorker;
}Info_Workers;

typedef struct datos_Reduccion{
	t_list* listaDeTemporales;
	char* archivoTemporal;
}datos_Reduccion;//estructura que se manda entre Master y los Workers


typedef struct saludo_datanode{
	int nombre_nodo;
	int capacidad_nodo;
	char* ip_worker;
}saludo_datanode;

typedef struct replanificacion{

	int numeroDeJOb;
	int nodoCaido;

}Replanificacion;


typedef struct script{
	char* nombre;
	char* contenido;
} script;

//Info que le manda el Master a Worker //serializacion y deserializacion probada y andando
typedef struct infoTransformacionParaWorker{
	int nodo;
	int bloque;
	int bytesOcupados;
	char* archivoTemporal;
} infoTransformacionParaWorker;

//INFO QUE LE MANDA MASTER A WORKER EN REDUCCION LOCAL //probando
typedef struct infoReduccionLocalParaWorker{
	t_list* listaDeArchivosTemporales;
	char* archivoTemporalReduccionLocal;
} infoReduccionLocalParaWorker;

//INFO QUE LE MANDA MASTER A WORKER EN REDUCCION GLOBAL
typedef struct infoReduccionGlobalDeMasterParaWorker{
	t_list* listaInfoParaReduccionGlobal; //Lista de estructura infoParaReduccionGLobal;
	char* archivoTemporalReduccionGlobal;
	script scriptReduccionGlobal;
} infoReduccionGlobalDeMasterParaWorker;



typedef struct infoParaReduccionGlobal{
	char* ipWorker;
	int puerto;
	char* archivoTemporalReduccionLocal;
}infoParaReduccionGlobal;

//INFO QUE LE MANDA EL WORKER ENCARGADO A WORKERS
typedef struct infoReduccionGlobalDeWorkerParaWorker{
	char* archivoTemporalLocalRequerido;
} infoReduccionGlobalDeWorkerParaWorker;

//ALMACENADO FINAL
typedef struct almacenadoFinal{
	char* archivoTemporalReduccionGlobal;
} almacenadoFinal;

typedef struct vectorConIndice{
	char** vector;
	int indice;
	int pasoDeDatosCompleto; //0 si no esta completo, 1 si esta completo.
}vectorConIndice;

typedef struct archivo{
	char* nombre;
	char* contenido;
}archivo;



t_list * list_info_workers;
t_list * listaUbicaciones;

//funciones

nodoParaPlanificar* crearNodoParaPlanificar(int nodo, int disponibilidad,
		int carga,int bloque);
void DestruirNodoParaPlanificar(nodoParaPlanificar* nodo);
RespuestaTransformacionYAMA* setRespuestaTransformacionYAMA(int nodo,
		int puertoWorker, char* ipWorker, int bloque, int bytesOcupados,
		char* archivoTemporal);
RespuestaReduccionLocal* crearRespuestaReduccionLocal(int nodo,int puertoWorker,
		char* ipWorker,t_list* archivosDeTransformacion, char* archivoReduccionLocal);
Paquete* crearPaquete(uint32_t codigo, uint32_t tamanio, char* mensaje);
void destruirPaquete(Paquete* package);
size_t sizePackage(Paquete *package);
char* serializarPaquete(Paquete *package);
int enviarPaquete(int fileDescriptor, Paquete *package);
/*
UbicacionBloquesArchivo* crearUbicacionBloquesArchivos(int parteDelArchivo,int bytesOcupados,int copia1Nodo, int copia1Bloque
		,int copia2Nodo,int copia2Bloque);
*/
void destruirUbicacionBloquesArchivo(UbicacionBloquesArchivo2* ubi);
JOBCompleto* crearJobCompleto(Job* job, t_list* listaDeUbicacionPartes,t_list* respuestaTransformacion);
int tamanioRespuestaTransformacionYAMA(t_list* listaDeRespuesta);
int tamanioJOB(Job* job);
RespuestaReduccionGlobal* crearRespuestaReduccionGlobal(int nodo, int puertoWorker, char* ipWorker,
		char* archivoReduccionLocal,char* archivoReduccionGlobal, bool encargado);
respuestaAlmacenadoFinal* crearRespuestaAlmacenadoFinal(int nodo,int puerto,char* ip,char* archivoReduccionGlobal);

infoReduccionLocalParaWorker* crearInfoReduccionLocalParaWorker(t_list* listaArchivosTemporales, char* archivoTemporal);
infoReduccionGlobalDeMasterParaWorker* crearInfoReduccionGlobalDeMasterParaWorker(t_list* listaArchivosReduccionLocal,char* archivoTemporalReduccionGlobal,script scritpReduccionGlobal);
infoParaReduccionGlobal* crearInfoParaReduccionGlobal(char* ipWorker,int puerto,char* archivoTemporalReduccionLocal);
infoReduccionGlobalDeWorkerParaWorker* crearInfoReduccionGlobalDeWorkerParaWorker(char* archivoTemporalLocalRequerido);
almacenadoFinal* crearAlmacenadoFinal(char* archivoTemporalReduccionGlobal);
UbicacionBloquesArchivo2* crearUbicacionBloquesArchivos2(int parteDelArchivo,int bytesOcupados,int copia1Nodo, int copia1Bloque,int copia2Nodo,int copia2Bloque);


#endif /* SO_LIB_ESTRUCTURAS_H_ */
