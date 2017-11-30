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

/*SERIALIZAR DE UN NODO/ESTRUCTURA FUNCIONANDO OK */
char* serializarRespuestaTransformacionYAMA(int nodo, int puertoWorker,
		char* ipWorker, int bloque, int bytesOcupados, char* archivoTemporal) {

	logInfo("Entró a la serializar una estructura\n");
	char* rtaSerializada = malloc(
			sizeof(int) * 6 + strlen(ipWorker)
					+ strlen(archivoTemporal));

	int tamanioIpWorker = strlen(ipWorker);
	int tamanioArchivoTemporal = strlen(archivoTemporal);
	int desplazamiento = 0;

	serializarDato(rtaSerializada, &(tamanioIpWorker), sizeof(int), &desplazamiento);
		logInfo("Serializo tamanio IpWorker");

	serializarDato(rtaSerializada, &(tamanioArchivoTemporal), sizeof(int), &desplazamiento);
		logInfo("Serializo tamanio del archivo temporal");

	serializarDato(rtaSerializada, &(nodo), sizeof(int), &desplazamiento);
	logInfo("Serializo nodo");
	serializarDato(rtaSerializada, &(puertoWorker), sizeof(int),
			&desplazamiento);
	serializarDato(rtaSerializada, &(bloque), sizeof(int), &desplazamiento);
	logInfo("Serializo bloque");
	serializarDato(rtaSerializada, &(bytesOcupados), sizeof(int),
			&desplazamiento);
	logInfo("Serializo bytes");
	serializarDato(rtaSerializada, ipWorker, strlen(ipWorker),
			&desplazamiento);
	logInfo("Serializo ip");
	serializarDato(rtaSerializada, archivoTemporal,
			strlen(archivoTemporal), &desplazamiento);
	logInfo("Serializo archivo");

	logInfo("Serializar respuesta transf yama bien");

	return (rtaSerializada);

}

/*DESERIALIZAR DE UN NODO/ESTRUCTURA FUNCIONANDO OK */
RespuestaTransformacionYAMA *deserializarRespuestaTransformacionYAMA(
		char* rtaSerializada) {

	logInfo("Entro a deserializar la estructura");
	int desplazamiento = 0;
	int tamanioIpWorker = malloc(sizeof(int));
	int tamanioArchivoTemporal = malloc(sizeof(int));


	deserializarDato(&(tamanioIpWorker),rtaSerializada, sizeof(int), &desplazamiento);
	logInfo("Entro a 1\nTamanio ip worker: %d", tamanioIpWorker);

	deserializarDato(&(tamanioArchivoTemporal),	rtaSerializada, sizeof(int), &desplazamiento);
		logInfo("Entro a 2\nTamanio archivo temporal: %d", tamanioArchivoTemporal);

	RespuestaTransformacionYAMA * respuestaTransformacionYAMA = malloc((sizeof(int)*4) + tamanioIpWorker + tamanioArchivoTemporal);

	deserializarDato(&(respuestaTransformacionYAMA->nodo),rtaSerializada, sizeof(int), &desplazamiento);
	logInfo("Entro a 3\nID NODO: %d",respuestaTransformacionYAMA->nodo);

	deserializarDato(&(respuestaTransformacionYAMA->puertoWorker),rtaSerializada, sizeof(int), &desplazamiento);
	logInfo("Entro a 4\nPuerto: %d",respuestaTransformacionYAMA->puertoWorker);

	deserializarDato(&(respuestaTransformacionYAMA->bloque), rtaSerializada,sizeof(int), &desplazamiento);
	logInfo("Entro a 5\nBloque: %d",respuestaTransformacionYAMA->bloque);

	deserializarDato(&(respuestaTransformacionYAMA->bytesOcupados),
			rtaSerializada, sizeof(int), &desplazamiento);
	logInfo("Entro a 6\nBytesocupados: %d",respuestaTransformacionYAMA->bytesOcupados);

	respuestaTransformacionYAMA->ipWorkwer = string_substring(rtaSerializada,desplazamiento,tamanioIpWorker);
	respuestaTransformacionYAMA->ipWorkwer[tamanioIpWorker +1] = "\0";


	logInfo("Entro a 7\nIP: %s",respuestaTransformacionYAMA->ipWorkwer);
	logInfo("LLego ip WORKER");
	desplazamiento+= tamanioIpWorker;

	respuestaTransformacionYAMA->archivoTemporal = string_substring(rtaSerializada,desplazamiento,tamanioArchivoTemporal);
	respuestaTransformacionYAMA->archivoTemporal[tamanioArchivoTemporal +1] = "\0";

	logInfo("Entro a 6\nARchivo: %s",respuestaTransformacionYAMA->archivoTemporal);
	desplazamiento+= tamanioArchivoTemporal;


	logInfo("LLego a archivo");

	logInfo("Respuesta archivo temporal: %s", respuestaTransformacionYAMA->archivoTemporal);
	logInfo("DESERIALIZAR COMPLETO");


	return (respuestaTransformacionYAMA);

}

int tamanioEstructuraRespuestaTransf(RespuestaTransformacionYAMA* nodo){
	int tamanio = (sizeof(int)*6) + strlen(nodo->archivoTemporal) + strlen(nodo->ipWorkwer);
	return tamanio;
}
/* FUNCIONA OK SERIALIZAR LISTA YAMA */
char* serializarListaYAMA(t_list* contextos){

	uint32_t total_size = tamanioRespuestaTransformacionYAMA(contextos);
	char *listaSerializada = malloc((sizeof(char)*total_size) + (list_size(contextos)*sizeof(uint32_t))+ sizeof(int));
	int offset = 0;
	int i;
	int tamanio_lista = list_size(contextos);
	serializarDato(listaSerializada,&(tamanio_lista),sizeof(int),&offset);
	for (i = 0; i < list_size(contextos); i++) {
		RespuestaTransformacionYAMA* nodo = list_get(contextos,i);
		char* contextoSerializado = serializarRespuestaTransformacionYAMA(nodo->nodo,
				nodo->puertoWorker, nodo->ipWorkwer, nodo->bloque,
				nodo->bytesOcupados, nodo->archivoTemporal);

		uint32_t size_contexto = tamanioEstructuraRespuestaTransf(list_get(contextos,i));
		serializarDato(listaSerializada,&(size_contexto),sizeof(uint32_t),&offset);//size contexto
		serializarDato(listaSerializada,contextoSerializado,sizeof(char)*size_contexto,&offset);//contexto
		free(contextoSerializado);

	}

	return listaSerializada;
}
/* FUNCIONA OK DESERIALIZAR LISTA YAMA */
t_list* deserializarListaYAMA(char* stackSerializado){
	int tamanio_lista= malloc(sizeof(int));

	int desplazamiento = 0;
	t_list* contextos = list_create();//malloc(sizeof(contexto) * tamanio_contexto);
	int i;

	// Se deserializa cada elemento del Stack
	deserializarDato(&(tamanio_lista),stackSerializado,sizeof(int),&desplazamiento);
	logInfo("tamnio_lista %i",tamanio_lista);
	int tamanio_contexto = tamanio_lista;

	for (i = 0; i < tamanio_contexto; i++) {

		uint32_t size_contexto;
		deserializarDato(&(size_contexto),stackSerializado,sizeof(uint32_t),&desplazamiento);

		char* contextoSerializado = malloc(sizeof(char)*size_contexto);
		deserializarDato(contextoSerializado,stackSerializado,size_contexto,&desplazamiento);

		RespuestaTransformacionYAMA* auxiliar = deserializarRespuestaTransformacionYAMA(contextoSerializado);
		list_add(contextos,auxiliar);

		//free(auxiliar);

		free(contextoSerializado);
	}
	//free(tamanio_lista);
	return contextos;

}

int tamanioScript(script* script){
	int contenido = strlen(script->contenido);
	int nombre = strlen(script->nombre);

	int tamanioScript = contenido + nombre;
	//logInfo("tamanioScript%i\n",tamanioScript);
	return tamanioScript;
}

char* serializarInfoParaWorker(int nodo, int bloque, int bytesOcupados,
		char* archivoTemporal) {

	logInfo("Entró a la serializar una estructua INFOPARAWORKER\n");
	int tamanioArchivoTemporal = strlen(archivoTemporal);

	logInfo("Se obtuvo: tamanioArchivoTemp: %i\n",tamanioArchivoTemporal);

	char* rtaSerializada = malloc(sizeof(int) * 5 + tamanioArchivoTemporal);
	int desplazamiento = 0;

	serializarDato(rtaSerializada, &(tamanioArchivoTemporal), sizeof(int), &desplazamiento);

	serializarDato(rtaSerializada, &(nodo), sizeof(int), &desplazamiento);

	serializarDato(rtaSerializada, &(bloque), sizeof(int), &desplazamiento);

	serializarDato(rtaSerializada, &(bytesOcupados), sizeof(int),
			&desplazamiento);

	serializarDato(rtaSerializada, archivoTemporal,tamanioArchivoTemporal, &desplazamiento);

	logInfo("Salgo de serializar una estructua INFOPARAWORKER\n");
	return (rtaSerializada);

}

infoParaWorker *deserializarInfoParaWorker(char* rtaSerializada) {

	logInfo("ENTRO A DESERIALIZAR INFOPARAWORKER");
	int desplazamiento = 0;
	int tamanioArchivoTemp =malloc(sizeof(int));

	deserializarDato(&(tamanioArchivoTemp), rtaSerializada, sizeof(int),&desplazamiento);

	infoParaWorker * respuesta = malloc(tamanioArchivoTemp+sizeof(int) * 3);

	deserializarDato(&(respuesta->nodo), rtaSerializada, sizeof(int),&desplazamiento);
	deserializarDato(&(respuesta->bloque), rtaSerializada, sizeof(int),&desplazamiento);
	deserializarDato(&(respuesta->bytesOcupados), rtaSerializada, sizeof(int),	&desplazamiento);

	respuesta->archivoTemporal = string_substring(rtaSerializada,desplazamiento,tamanioArchivoTemp);
	desplazamiento=desplazamiento+tamanioArchivoTemp;

	logInfo("DESERIALIZAR COMPLETO INFOPARAWORKER");
	free(rtaSerializada);
	return (respuesta);

}

char* serializarScript(script* script) {
	char* nombre = script->nombre;
	char* contenido = script->contenido;
	int tamanioContenido = strlen(contenido);
	int tamanioNombre = strlen(nombre);

	char* scriptSerializado = malloc(tamanioNombre + tamanioContenido + sizeof(int)*2);
	int desplazamiento = 0;

	serializarDato(scriptSerializado, &(tamanioNombre), sizeof(int), &desplazamiento);
	serializarDato(scriptSerializado, &(tamanioContenido), sizeof(int), &desplazamiento);

	serializarDato(scriptSerializado, contenido,tamanioContenido, &desplazamiento);
	serializarDato(scriptSerializado, nombre, tamanioNombre,&desplazamiento);

	return scriptSerializado;
}

script* deserilizarScript(char* bloqueSerializado) {

	int desplazamiento = 0;
	int tamanioContenido;
	int tamanioNombre;
	deserializarDato(&(tamanioNombre), bloqueSerializado,sizeof(int), &desplazamiento);
	deserializarDato(&(tamanioContenido), bloqueSerializado,sizeof(int), &desplazamiento);

	script* scriptDeserializado = malloc(tamanioContenido+tamanioNombre);

	scriptDeserializado->contenido = string_substring(bloqueSerializado,desplazamiento, tamanioContenido);
	desplazamiento = desplazamiento+ tamanioContenido;

	scriptDeserializado->nombre = string_substring(bloqueSerializado,desplazamiento,tamanioNombre);
	desplazamiento = desplazamiento+ tamanioNombre;

	free(bloqueSerializado);

	return scriptDeserializado;
}
////////////////////////////////////////////////////////////////////////////////////////

int deserializarINT(char* stream) {
	size_t size = sizeof(int);
	int value;
	memcpy(&value, stream, size);
	return value;
}


char* serializarBloque(int numeroBloque, char* contenidoBloque) {

	char *bloqueSerializado = malloc(
			strlen(contenidoBloque) + sizeof(int)*2);
	int desplazamiento = 0;
	int tamanio = strlen(contenidoBloque);

	serializarDato(bloqueSerializado, &(numeroBloque), sizeof(int),
			&desplazamiento);

	serializarDato(bloqueSerializado, &(tamanio), sizeof(int),
			&desplazamiento);

	serializarDato(bloqueSerializado, contenidoBloque,
			strlen(contenidoBloque), &desplazamiento);

	return bloqueSerializado;
}

SetBloque* deserilizarBloque(char* bloqueSerializado) {
	SetBloque* setbloque = malloc(1024 * 1024 + sizeof(int)*2);
	int desplazamiento = 0;
	int tamanioContenido;
	deserializarDato(&(setbloque->nrobloque), bloqueSerializado, sizeof(int),
			&desplazamiento);
	deserializarDato(&(tamanioContenido), bloqueSerializado, sizeof(int),
			&desplazamiento);
	setbloque->contenidoBloque = string_substring(bloqueSerializado,desplazamiento,tamanioContenido);

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

	uint32_t total_size = tamanioEstructurasListaWorkers(listaWorkers);
	char *listaSerializada = malloc((sizeof(char)*total_size) + list_size(listaWorkers)*sizeof(uint32_t)+ sizeof(int));
	int offset = 0;
	int i;
	int tamanio_lista = list_size(listaWorkers);
	serializarDato(listaSerializada,&(tamanio_lista),sizeof(int),&offset);
	for (i = 0; i < list_size(listaWorkers); i++) {
		Info_Workers* info = list_get(listaWorkers,i);
		char* contextoSerializado = serializarInfoWorker(info->puerto,info->ipWorker);
		uint32_t size_contexto = tamanioEstructuraInfoWorker(list_get(listaWorkers,i)) +sizeof(int);
		serializarDato(listaSerializada,&(size_contexto),sizeof(uint32_t),&offset);//size contexto
		serializarDato(listaSerializada,contextoSerializado,sizeof(char)*size_contexto,&offset);//contexto
	//    free(contextoSerializado);

	}

	return listaSerializada;
}

t_list * deserializarLista_info_workers(char * listaWorkersSerializada) {
	int tamanio_lista= malloc(sizeof(int));

		int desplazamiento = 0;
		t_list* lista = list_create();
		int i;

		// Se deserializa cada elemento
		deserializarDato(&(tamanio_lista),listaWorkersSerializada,sizeof(int),&desplazamiento);
		//logInfo("tamnio_lista %d",tamanio_lista);
		int tamanio_contexto = tamanio_lista;

		for (i = 0; i < tamanio_contexto; i++) {

			uint32_t size_contexto;
			deserializarDato(&(size_contexto),listaWorkersSerializada,sizeof(uint32_t),&desplazamiento);

			char* infoWorkerSerializado = malloc(sizeof(char)*size_contexto);
			deserializarDato(infoWorkerSerializado,listaWorkersSerializada,size_contexto,&desplazamiento);

			Info_Workers* auxiliar = deserializarInfoWorker(infoWorkerSerializado);
			list_add(lista,auxiliar);

			//free(auxiliar);

			//free(infoWorkerSerializado);
		}
		//free(tamanio_lista);
		return lista;}

char* serializarInfoWorker(int puerto, char* ipWorker) {
	char* infoWorkerSerializado = malloc(
			strlen(ipWorker) + sizeof(int)*2);
	int desplazamiento = 0;
	int tamanioIP = strlen(ipWorker);
	serializarDato(infoWorkerSerializado, &(tamanioIP), sizeof(int), &desplazamiento);
	serializarDato(infoWorkerSerializado, &(puerto), sizeof(int),
			&desplazamiento);
	serializarDato(infoWorkerSerializado, ipWorker,
			strlen(ipWorker)*sizeof(char), &desplazamiento);


	return infoWorkerSerializado;

}

Info_Workers *deserializarInfoWorker(char * infoWorkerSerializado) {

	int tamanioIP = malloc(sizeof(int));
	int desplazamiento = 0;

	deserializarDato(&(tamanioIP), infoWorkerSerializado, sizeof(int),
				&desplazamiento);

	Info_Workers *infoworker = malloc(tamanioIP + sizeof(int));

	deserializarDato(&(infoworker->puerto), infoWorkerSerializado, sizeof(int),
			&desplazamiento);
	infoworker->ipWorker = string_substring(infoWorkerSerializado,desplazamiento,tamanioIP);
	desplazamiento+=tamanioIP;

	//free(tamanioIP);
	free(infoWorkerSerializado);
	return infoworker;

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

		strcat(ListaSerializada, contenido);
	}
	return (ListaSerializada);
}

char * serializar_saludo(int nombre_nodo, int capacidad_nodo, char* ipWorker) {
	char *saludo_serializado = malloc(
			sizeof(char) + sizeof(int) * 2 + strlen(ipWorker));
	int desplazamiento = 0;

	serializarDato(saludo_serializado, &(nombre_nodo), sizeof(int),
			&desplazamiento);

	serializarDato(saludo_serializado, &(capacidad_nodo), sizeof(int),
			&desplazamiento);

	serializarDato(saludo_serializado, ipWorker,
			strlen(ipWorker) + sizeof(char), &desplazamiento);

	return saludo_serializado;

}

saludo_datanode *deserializar_saludo_datanode(char* saludoSerializado) {
	saludo_datanode* saludo = malloc(40);
	int desplazamiento = 0;

	deserializarDato(&(saludo->nombre_nodo), saludoSerializado, sizeof(int),
			&desplazamiento);

	deserializarDato(&(saludo->capacidad_nodo), saludoSerializado, sizeof(int),
			&desplazamiento);

	saludo->ip_worker = strdup(saludoSerializado + desplazamiento);

	return saludo;

}









