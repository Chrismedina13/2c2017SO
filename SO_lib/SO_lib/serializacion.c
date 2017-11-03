#include "serializacion.h"
#include "estructuras.h"

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

int deserializarINT(char* stream){
	size_t size = sizeof(int);
	int value;
	memcpy(&value, stream, size);
	return value;
}



char* serializarBloque(SetBloque * setbloque){

		char *bloqueSerializado= malloc(sizeof(setbloque->contenidoBloque) + sizeof(setbloque->nrobloque));
		int desplazamiento = 0;
		serializarDato(bloqueSerializado,&(setbloque->nrobloque),sizeof(int),&desplazamiento);
		serializarDato(bloqueSerializado,&(setbloque->contenidoBloque),sizeof(setbloque->nrobloque),&desplazamiento);

		return bloqueSerializado;
}


SetBloque * deserilizarBloque(char* bloqueSerializado){
	SetBloque * setbloque = malloc(sizeof(int)+ 1024*1024);
	int desplazamiento = 0;
	deserializarDato(setbloque->nrobloque,bloqueSerializado,sizeof(int),&desplazamiento);
	deserializarDato(setbloque->contenidoBloque,bloqueSerializado,1024*1024,&desplazamiento);

return setbloque;
}


char* serializarUbicacionBloque( UbicacionBloque * ubicacionbloque){
	char* ubicacionBloqueSerializado = malloc(sizeof(int)*2);
	int desplazamiento =0 ;
	serializarDato(ubicacionBloqueSerializado, &(ubicacionbloque->nodo), sizeof(int), &desplazamiento);
	serializarDato(ubicacionBloqueSerializado,&(ubicacionbloque->bloqueDelNodoDeLaCopia), sizeof(int), &desplazamiento);
	return ubicacionBloqueSerializado;
}

char * serializarUblicacionBloqueArchivo(UbicacionBloquesArchivo * estructura){
char* estructuraSerializada = malloc(sizeof(int)*6);
int desplazamiento=0;
serializarDato(estructuraSerializada, &(estructura->parteDelArchivo), sizeof(int), &desplazamiento);
serializarUbicacionBloque(estructuraSerializada, &(estructura->ubicacionCopia1), sizeof(int)*2, &desplazamiento);
serializarUbicacionBloque(estructuraSerializada, &(estructura->ubicacionCopia2), sizeof(int)*2, &desplazamiento);
serializarDato(estructuraSerializada, &(estructura->bytesOcupados),sizeof(int), &desplazamiento);
		return estructuraSerializada;
}


UbicacionBloque deserializarUbicacionBloque(char* ubicacionbloqueserializado){
	UbicacionBloque * ubicacionbloque = malloc(sizeof(int)*2);
	int desplazamiento=0;
	deserializarDato(ubicacionbloque->nodo, ubicacionbloqueserializado,sizeof(int), &desplazamiento);
	deserializarDato(ubicacionbloque->bloqueDelNodoDeLaCopia, ubicacionbloqueserializado, sizeof(int), &desplazamiento);
	return ubicacionbloque;
}

UbicacionBloquesArchivo deserializarUbicacionArchivo(char* UbicacionSerializada){
UbicacionBloquesArchivo * ubicacionbloquesarchivos = malloc(sizeof(int)*6);
int desplazamiento=0;
deserializarDato(ubicacionbloquesarchivos->parteDelArchivo, UbicacionSerializada, sizeof(int), &desplazamiento);
deserializarUbicacionBloque(ubicacionbloquesarchivos->ubicacionCopia1, UbicacionSerializada, sizeof(int)*2,&desplazamiento);
deserializarUbicacionBloque(ubicacionbloquesarchivos->ubicacionCopia2, UbicacionSerializada, sizeof(int)*2, &desplazamiento);
deserializarDato(ubicacionbloquesarchivos->bytesOcupados, UbicacionSerializada, sizeof(int), &desplazamiento);

return ubicacionbloquesarchivos;
}
