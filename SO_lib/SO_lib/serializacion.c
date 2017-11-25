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

char* serializarRespuestaTransformacionYAMA(int nodo, int puertoWorker,
		char* ipWorker, int bloque, int bytesOcupados, char* archivoTemporal) {

	loginfo("Serializadando el primer nodo");
	char* rtaSerializada = malloc((sizeof(int) * 4 + sizeof(char*) * 2));
	int desplazamiento = 0;
	serializarDato(rtaSerializada, &(nodo), sizeof(int), &desplazamiento);

	serializarDato(rtaSerializada, &(puertoWorker), sizeof(int),

			&desplazamiento);
	serializarDato(rtaSerializada, &(bloque), sizeof(int), &desplazamiento);

	serializarDato(rtaSerializada, &(bytesOcupados), sizeof(int),
			&desplazamiento);

	serializarDato(rtaSerializada, &(ipWorker), sizeof(char*), &desplazamiento);

	serializarDato(rtaSerializada, &(archivoTemporal), sizeof(char*),
			&desplazamiento);
	loginfo("Serializar respuesta transf yama bien");
	return (rtaSerializada);

}

RespuestaTransformacionYAMA *deserializarRespuestaTransformacionYAMA(
		char* rtaSerializada) {
	RespuestaTransformacionYAMA * respuestaTransformacionYAMA = malloc(
			(sizeof(int) * 4 + sizeof(char*) * 2));
	int desplazamiento = 0;
	respuestaTransformacionYAMA->nodo = strdup(rtaSerializada + desplazamiento);

	deserializarDato(&(respuestaTransformacionYAMA->puertoWorker),
			rtaSerializada, sizeof(int), &desplazamiento);

	deserializarDato(&(respuestaTransformacionYAMA->bloque), rtaSerializada,
				sizeof(int), &desplazamiento);


	deserializarDato(&(respuestaTransformacionYAMA->bytesOcupados),
				rtaSerializada, sizeof(int), &desplazamiento);

	respuestaTransformacionYAMA->ipWorkwer = strdup(
			rtaSerializada + desplazamiento);

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
		nodo = list_get(listaWorkers, i);
		strcat(ListaSerializada,
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
	loginfo("llego hasta aca 1");
	char* algo;
	for (i = 0; list_size(lista); i++) {
		nodo = list_get(lista, i);
		algo = serializarRespuestaTransformacionYAMA(nodo->nodo,nodo->puertoWorker,
				nodo->ipWorkwer, nodo->bloque,
				nodo->bytesOcupados, nodo->archivoTemporal);
		string_append(ListaSerializada,algo);
		algo = '\0';
	}

	loginfo("llego hasta aca 2");
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

char* serializarFinTransformacion(finTransformacion* fin) {

	int desplazamiento = 0;
	char* finTransformcaionSerializado = malloc(sizeof(int) * 2);
	serializarDato(finTransformcaionSerializado, &(fin->nodo), sizeof(int),
			&desplazamiento);
	serializarDato(finTransformcaionSerializado, &(fin->numeroDeJob),
			sizeof(int), &desplazamiento);
	return (finTransformcaionSerializado);
}

finTransformacion * deserializarFinTransformacion(char* FT) {

	int desplazamiento;
	finTransformacion* fin = malloc(sizeof(int) * 2);
	deserializarDato(&(FT), fin->nodo, sizeof(int), &desplazamiento);
	deserializarDato(&(FT), fin->numeroDeJob, sizeof(int), &desplazamiento);
	return fin;

}

char * serializarListaTemp(t_list * lista) {
	int i;
	char* ListaSerializada;
	char * contenido;

	for (i = 0; list_size(lista); i++) {

		contenido = list_get(lista, i);

		strcat(ListaSerializada,contenido);
	}
	return (ListaSerializada);
}
//falta hacer la deserrealizar de la lista de char*
/*t_list * deserializarListaTemp(char * lista) {
	int i;
	t_list * Lista;

	char * despl;
	for (i = 0; i<sizeof(lista); (i + sizeof(datos_Reduccion))) {
		despl = string_substring(lista, i,
				sizeof(Info_Workers));
		datos_Reduccion = deserializarInfoWorker(despl);
		list_add(Lista, infoworkers);
	}

	return (Lista);
}*/


char * serializar_saludo(int nombre_nodo, int capacidad_nodo, char* ipWorker ){
	char *saludo_serializado= malloc(sizeof(char) + sizeof(int)*2 + strlen(ipWorker));
		int desplazamiento = 0;

		serializarDato(saludo_serializado, &(nombre_nodo), sizeof(int),
				&desplazamiento);

		serializarDato(saludo_serializado, &(capacidad_nodo), sizeof(int),
					&desplazamiento);


			serializarDato(saludo_serializado, ipWorker,
				strlen(ipWorker) + sizeof(char), &desplazamiento);

		return saludo_serializado;

}


saludo_datanode *deserializar_saludo_datanode(char* saludoSerializado){
	saludo_datanode* saludo = malloc(40);
	int desplazamiento = 0;



	deserializarDato(&(saludo->nombre_nodo),saludoSerializado, sizeof(int),
			&desplazamiento);

	deserializarDato(&(saludo->capacidad_nodo), saludoSerializado, sizeof(int),
					&desplazamiento);



		saludo->ip_worker = strdup(saludoSerializado + desplazamiento);

	return saludo;

}


char* serializarInfoParaWorker(int nodo, int bloque, int bytesOcupados, char* archivoTemporal,script* scriptTransformacion) {
	char* rtaSerializada = malloc((sizeof(int) * 3 + sizeof(char*)));
	int desplazamiento = 0;

	serializarDato(rtaSerializada, &(nodo), sizeof(int), &desplazamiento);

	serializarDato(rtaSerializada, &(bloque), sizeof(int), &desplazamiento);

	serializarDato(rtaSerializada, &(bytesOcupados), sizeof(int),
			&desplazamiento);

	serializarDato(rtaSerializada, &(archivoTemporal), string_length(archivoTemporal),
			&desplazamiento);

	char* scriptSerializado = serializarScript(scriptTransformacion);
	serializarDato(rtaSerializada,scriptSerializado,string_length(scriptSerializado),&desplazamiento);

	return (rtaSerializada);

}

infoParaWorker *deserializarInfoParaWorker(
		char* rtaSerializada) {

	infoParaWorker * respuesta = malloc(sizeof(int) * 3 + sizeof(char*) + sizeof(script));

	int desplazamiento = 0;

	respuesta->nodo = strdup(rtaSerializada + desplazamiento);


	deserializarDato(&(respuesta->bloque), rtaSerializada,
				sizeof(int), &desplazamiento);


	deserializarDato(&(respuesta->bytesOcupados),
				rtaSerializada, sizeof(int), &desplazamiento);


	respuesta->archivoTemporal = strdup(
			rtaSerializada + desplazamiento);



	return (respuesta);

}


char* serializarScript(script* script){
	char* nombre = script->nombre;
	char* contenido = script->contenido;
	int tamanio = script->tamanio;
	char* scriptSerializado = malloc(strlen(nombre) + strlen(contenido) + sizeof(char)*2 + sizeof(int));
	int desplazamiento = 0;
	serializarDato(scriptSerializado, &(tamanio), sizeof(int),&desplazamiento);
	serializarDato(scriptSerializado, contenido,strlen(contenido) + sizeof(char), &desplazamiento);
	serializarDato(scriptSerializado, nombre,strlen(nombre) + sizeof(char), &desplazamiento);

	return scriptSerializado;
}

script* deserilizarScript(char* bloqueSerializado) {
	script* scriptDeserializado = malloc(1024 * 1024 + 4);
	int desplazamiento = 0;
	deserializarDato(&(scriptDeserializado->tamanio), bloqueSerializado, sizeof(int),&desplazamiento);
	scriptDeserializado->contenido = string_substring(bloqueSerializado, sizeof(int), scriptDeserializado->tamanio + sizeof(int));
	scriptDeserializado->nombre = string_substring(bloqueSerializado, sizeof(int) + string_length(scriptDeserializado->contenido), string_length(bloqueSerializado));
	return scriptDeserializado;
}



