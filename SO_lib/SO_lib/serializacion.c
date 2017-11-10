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

int deserializarINT(char* stream){
	size_t size = sizeof(int);
	int value;
	memcpy(&value, stream, size);
	return value;
}



char* serializarBloque(SetBloque * setbloque){

		char *bloqueSerializado= malloc(strlen(setbloque->contenidoBloque)+sizeof(char) + sizeof(setbloque->nrobloque));
		int desplazamiento = 0;
		serializarDato(bloqueSerializado,&(setbloque->nrobloque),sizeof(int),&desplazamiento);
		serializarDato(bloqueSerializado,&(setbloque->contenidoBloque),strlen(setbloque->contenidoBloque)+sizeof(char),&desplazamiento);

		return bloqueSerializado;
}


SetBloque*  deserilizarBloque(char* bloqueSerializado){
	SetBloque* setbloque = malloc(1024*1024 + 4);
	int desplazamiento = 0;
	deserializarDato(&(setbloque->nrobloque),bloqueSerializado,sizeof(int),&desplazamiento);
	setbloque->contenidoBloque = strdup(bloqueSerializado + desplazamiento);


return setbloque;
}


char* serializarUbicacionBloque( UbicacionBloque  ubicacionbloque){
	char* ubicacionBloqueSerializado = malloc(sizeof(int)*2);
	int desplazamiento =0 ;
	serializarDato(ubicacionBloqueSerializado, &(ubicacionbloque.nodo), sizeof(int), &desplazamiento);
	serializarDato(ubicacionBloqueSerializado,&(ubicacionbloque.bloqueDelNodoDeLaCopia), sizeof(int), &desplazamiento);
	return ubicacionBloqueSerializado;
}

char * serializarUblicacionBloqueArchivo(UbicacionBloquesArchivo * estructura){
	char* estructuraSerializada = malloc(sizeof(int)*6);
	int desplazamiento=0;
	serializarDato(estructuraSerializada, &(estructura->parteDelArchivo), sizeof(int), &desplazamiento);
	serializarDato(estructuraSerializada, &(estructura->bytesOcupados),sizeof(int), &desplazamiento);
	strcat(estructuraSerializada, serializarUbicacionBloque(estructura->ubicacionCopia1));
	strcat(estructuraSerializada, serializarUbicacionBloque(estructura->ubicacionCopia2));

	serializarDato(estructuraSerializada, &(estructura->bytesOcupados),sizeof(int), &desplazamiento);
		return estructuraSerializada;
}


UbicacionBloque deserializarUbicacionBloque(char* ubicacionbloqueserializado){
	UbicacionBloque ubicacionbloque;
	int desplazamiento=0;
	deserializarDato(&(ubicacionbloque.nodo), ubicacionbloqueserializado,sizeof(int), &desplazamiento);
	deserializarDato(&(ubicacionbloque.bloqueDelNodoDeLaCopia), ubicacionbloqueserializado, sizeof(int), &desplazamiento);

	return ubicacionbloque;

}


UbicacionBloquesArchivo *deserializarUbicacionArchivo(char* UbicacionSerializada){
UbicacionBloquesArchivo * ubicacionbloquesarchivos = malloc(sizeof(int)*6);
int desplazamiento=0;
deserializarDato(&(ubicacionbloquesarchivos->parteDelArchivo), UbicacionSerializada, sizeof(int), &desplazamiento);
deserializarDato(&(ubicacionbloquesarchivos->bytesOcupados), UbicacionSerializada, sizeof(int), &desplazamiento);
ubicacionbloquesarchivos->ubicacionCopia1=deserializarUbicacionBloque(UbicacionSerializada);
ubicacionbloquesarchivos->ubicacionCopia2=deserializarUbicacionBloque(UbicacionSerializada);


return ubicacionbloquesarchivos;
}

char* serializarRespuestaTransformacionYAMA(RespuestaTransformacionYAMA rtaTransformacionYAMA){
	char* rtaSerializada =  malloc((sizeof(int)*3 + sizeof(char*)*3));
	int desplazamiento =0;
	serializarDato(rtaSerializada, &(rtaTransformacionYAMA.nodo), sizeof(char*), &desplazamiento);
	serializarDato(rtaSerializada, &(rtaTransformacionYAMA.puertoWorker), sizeof(int), &desplazamiento);
	serializarDato(rtaSerializada, &(rtaTransformacionYAMA.ipWorkwer), sizeof(char*), &desplazamiento);
	serializarDato(rtaSerializada, &(rtaTransformacionYAMA.bloque), sizeof(int), &desplazamiento);
	serializarDato(rtaSerializada, &(rtaTransformacionYAMA.bytesOcupados), sizeof(int), &desplazamiento);
	serializarDato(rtaSerializada, &(rtaTransformacionYAMA.archivoTemporal), sizeof(char*), &desplazamiento);

return(rtaSerializada);


}

/*RespuestaTransformacionYAMA  *deserializarRespuestaTransformacionYAMA(char* rtaSerializada){
	RespuestaTransformacionYAMA * respuestaTransformacionYAMA =  malloc((sizeof(int)*3 + sizeof(char*)*3));
	int desplazamiento = 0;
	deserializarDato(&(respuestaTransformacionYAMA.nodo), rtaSerializada, sizeof(char*), &desplazamiento);
	deserializarDato(&(respuestaTransformacionYAMA.puertoWorker), rtaSerializada, sizeof(int), &desplazamiento);
	deserializarDato(&(respuestaTransformacionYAMA.ipWorkwer), rtaSerializada, sizeof(char*), &desplazamiento);
	deserializarDato(&(respuestaTransformacionYAMA.bloque), rtaSerializada, sizeof(int), &desplazamiento);
	deserializarDato(&(respuestaTransformacionYAMA.bytesOcupados), rtaSerializada, sizeof(int), &desplazamiento);
	deserializarDato(&(respuestaTransformacionYAMA.archivoTemporal), rtaSerializada, sizeof(char*), &desplazamiento);

	return(respuestaTransformacionYAMA);

}
*/
