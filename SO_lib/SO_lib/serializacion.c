#include "serializacion.h"
#include "estructuras.h"
#include "commons/string.h"

void serializarDato(char* buffer, void* datoASerializar, int tamanio,
		int* offset) {
	memcpy(buffer + *offset, datoASerializar, tamanio);
	*offset += tamanio;
}

void deserializarDato(void* datoSerializado, char* buffer, int tamanio,
		int* offset) {
	memcpy(datoSerializado, buffer + *offset, tamanio);
	*offset += tamanio;
}

int deserializarINT(char* stream) {
	size_t size = sizeof(int);
	int value;
	memcpy(&value, stream, size);
	return value;
}

char* serializarBloque(int numeroBloque, char* contenidoBloque) {

	char *bloqueSerializado = malloc(
			strlen(contenidoBloque) + sizeof(char) + sizeof(int));
	int desplazamiento = 0;
	serializarDato(bloqueSerializado, &(numeroBloque), sizeof(int),
			&desplazamiento);
	serializarDato(bloqueSerializado, contenidoBloque,
			strlen(contenidoBloque) + sizeof(char), &desplazamiento);

	return bloqueSerializado;
}

SetBloque* deserilizarBloque(char* bloqueSerializado) {
	SetBloque* setbloque = malloc(1024 * 1024 + 4);
	int desplazamiento = 0;
	deserializarDato(&(setbloque->nrobloque), bloqueSerializado, sizeof(int),
			&desplazamiento);
	setbloque->contenidoBloque = strdup(bloqueSerializado + desplazamiento);

	return setbloque;
}

char* serializarUbicacionBloque(UbicacionBloque ubicacionbloque) {
	char* ubicacionBloqueSerializado = malloc(sizeof(int) * 2);
	int desplazamiento = 0;
	serializarDato(ubicacionBloqueSerializado, &(ubicacionbloque.nodo),
			sizeof(int), &desplazamiento);
	serializarDato(ubicacionBloqueSerializado,
			&(ubicacionbloque.desplazamiento), sizeof(int), &desplazamiento);
	return ubicacionBloqueSerializado;
}

char * serializarUblicacionBloqueArchivo(UbicacionBloquesArchivo * estructura) {
	char* estructuraSerializada = malloc(sizeof(int) * 6);
	int desplazamiento = 0;
	serializarDato(estructuraSerializada, &(estructura->parteDelArchivo),
			sizeof(int), &desplazamiento);
	serializarDato(estructuraSerializada, &(estructura->bytesOcupados),
			sizeof(int), &desplazamiento);
	strcat(estructuraSerializada,
			serializarUbicacionBloque(estructura->ubicacionCopia1));
	strcat(estructuraSerializada,
			serializarUbicacionBloque(estructura->ubicacionCopia2));

	serializarDato(estructuraSerializada, &(estructura->bytesOcupados),
			sizeof(int), &desplazamiento);
	return estructuraSerializada;
}

UbicacionBloque deserializarUbicacionBloque(char* ubicacionbloqueserializado) {
	UbicacionBloque ubicacionbloque;
	int desplazamiento = 0;
	deserializarDato(&(ubicacionbloque.nodo), ubicacionbloqueserializado,
			sizeof(int), &desplazamiento);
	deserializarDato(&(ubicacionbloque.desplazamiento),
			ubicacionbloqueserializado, sizeof(int), &desplazamiento);

	return ubicacionbloque;

}

UbicacionBloquesArchivo *deserializarUbicacionArchivo(
		char* UbicacionSerializada) {
	UbicacionBloquesArchivo * ubicacionbloquesarchivos = malloc(
			sizeof(int) * 6);
	int desplazamiento = 0;
	deserializarDato(&(ubicacionbloquesarchivos->parteDelArchivo),
			UbicacionSerializada, sizeof(int), &desplazamiento);
	deserializarDato(&(ubicacionbloquesarchivos->bytesOcupados),
			UbicacionSerializada, sizeof(int), &desplazamiento);
	ubicacionbloquesarchivos->ubicacionCopia1 = deserializarUbicacionBloque(
			UbicacionSerializada);
	ubicacionbloquesarchivos->ubicacionCopia2 = deserializarUbicacionBloque(
			UbicacionSerializada);

	return ubicacionbloquesarchivos;
}

char* serializarRespuestaTransformacionYAMA(char* nodo, int puertoWorker,
		char* ipWorker, int bloque, int bytesOcupados, char* archivoTemporal) {
	char* rtaSerializada = malloc((sizeof(int) * 3 + sizeof(char*) * 3));
	int desplazamiento = 0;
	serializarDato(rtaSerializada, &(nodo), sizeof(char*), &desplazamiento);
	serializarDato(rtaSerializada, &(puertoWorker), sizeof(int),
			&desplazamiento);
	serializarDato(rtaSerializada, &(ipWorker), sizeof(char*), &desplazamiento);
	serializarDato(rtaSerializada, &(bloque), sizeof(int), &desplazamiento);
	serializarDato(rtaSerializada, &(bytesOcupados), sizeof(int),
			&desplazamiento);
	serializarDato(rtaSerializada, &(archivoTemporal), sizeof(char*),
			&desplazamiento);

	return (rtaSerializada);

}

RespuestaTransformacionYAMA *deserializarRespuestaTransformacionYAMA(
		char* rtaSerializada) {
	RespuestaTransformacionYAMA * respuestaTransformacionYAMA = malloc(
			(sizeof(int) * 3 + sizeof(char*) * 3));
	int desplazamiento = 0;
	respuestaTransformacionYAMA->nodo = strdup(rtaSerializada + desplazamiento);
	deserializarDato(&(respuestaTransformacionYAMA->puertoWorker),
			rtaSerializada, sizeof(int), &desplazamiento);
	respuestaTransformacionYAMA->ipWorkwer = strdup(
			rtaSerializada + desplazamiento);
	deserializarDato(&(respuestaTransformacionYAMA->bloque), rtaSerializada,
			sizeof(int), &desplazamiento);
	deserializarDato(&(respuestaTransformacionYAMA->bytesOcupados),
			rtaSerializada, sizeof(int), &desplazamiento);
	respuestaTransformacionYAMA->archivoTemporal = strdup(
			rtaSerializada + desplazamiento);

	return (respuestaTransformacionYAMA);

}

char* serializarListaUbicacionBloquesArchivos(t_list * listaUbicaciones) {
	int i;
	char* listaSerializada;
	UbicacionBloquesArchivo * nodo;

	for (i = 0; list_size(listaUbicaciones); i++) {
		nodo = list_get((listaUbicaciones), i);
		listaSerializada = strcat(listaSerializada,
				serializarUblicacionBloqueArchivo(nodo));

	}
	return listaSerializada;

}

t_list * deserializarUbicacionBloquesArchivos(char* ListaUbicacionesSerializada) {
	int i;
	t_list * Lista;
	UbicacionBloquesArchivo * ubicacionbloquearchivo;
	char * despl;

	for (i = 0; sizeof(ListaUbicacionesSerializada);
			(i + sizeof(UbicacionBloquesArchivo))) {
		despl = string_substring(ListaUbicacionesSerializada, i,
				sizeof(UbicacionBloquesArchivo));
		ubicacionbloquearchivo = deserializarUbicacionArchivo(despl);
		list_add(Lista, ubicacionbloquearchivo);
	}
	return (Lista);
}

char * serializarLista_info_workers(t_list * listaWorkers) {
	int i;
	char* ListaSerializada;
	Info_Workers * nodo;

	for (i = 0; list_size(listaWorkers); i++) {
		nodo = list_get(ListaSerializada, i);
		ListaSerializada = strcat(ListaSerializada,
				serializarInfoWorker(nodo->puerto, nodo->ipWorker));
	}
	return (ListaSerializada);
}

t_list * deserializarLista_info_workers(char * listaWorkersSerializada) {
	int i;
	t_list * Lista;
	Info_Workers * infoworkers;
	char * despl;
	for (i = 0; sizeof(listaWorkersSerializada); (i + sizeof(Info_Workers))) {
		despl = string_substring(listaWorkersSerializada, i,
				sizeof(Info_Workers));
		infoworkers = deserializarInfoWorker(despl);
		list_add(Lista, infoworkers);
	}

	return (Lista);
}

char* serializarInfoWorker(int puerto, char* ipWorker) {
	char* infoWorkerSerializado = malloc(
			strlen(ipWorker) + sizeof(char) + sizeof(int));
	int desplazamiento = 0;
	serializarDato(infoWorkerSerializado, &(puerto), sizeof(int),
			&desplazamiento);
	serializarDato(infoWorkerSerializado, ipWorker,
			strlen(ipWorker) + sizeof(char), &desplazamiento);

	return infoWorkerSerializado;

}

Info_Workers *deserializarInfoWorker(char * infoWorkerSerializado) {
	Info_Workers *infoworker = malloc(11 + sizeof(char) + sizeof(int));
	int desplazamiento = 0;
	deserializarDato(&(infoworker->puerto), infoWorkerSerializado, sizeof(int),
			&desplazamiento);
	infoworker->ipWorker = strdup(infoWorkerSerializado + desplazamiento);

	return infoworker;

}
char * serializarListaRespuestaTransf(t_list * lista) {
	int i;
	char* ListaSerializada;
	RespuestaTransformacionYAMA * nodo;

	for (i = 0; list_size(lista); i++) {
		nodo = list_get(ListaSerializada, i);
		ListaSerializada = strcat(ListaSerializada,
				serializarRespuestaTransformacionYAMA(nodo->nodo,
						nodo->puertoWorker, nodo->ipWorkwer, nodo->bloque,
						nodo->bytesOcupados, nodo->archivoTemporal));
	}
	return (ListaSerializada);
}
t_list * deserializarListaRespuestaTransf(char * listaSerializada) {
	int i;
	t_list * Lista;
	RespuestaTransformacionYAMA * nodo;
	char * despl;
	for (i = 0; sizeof(listaSerializada);
			(i + sizeof(RespuestaTransformacionYAMA))) {
		despl = string_substring(listaSerializada, i,
				sizeof(RespuestaTransformacionYAMA));
		nodo = deserializarRespuestaTransformacionYAMA(despl);
		list_add(Lista, nodo);
	}

	return (Lista);
}
