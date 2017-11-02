#include "serializacion.h"

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


SetBloque deserilizarBloque(char* bloqueSerializado){
	SetBloque * setbloque = malloc(sizeof(int)+ 1024*1024);
	int desplazamiento = 0;
	deserializarDato(setbloque->nrobloque,bloqueSerializado,sizeof(int),&desplazamiento);
	deserializarDato(setbloque->contenidoBloque,bloqueSerializado,1024*1024,&desplazamiento);

return setbloque;
}


