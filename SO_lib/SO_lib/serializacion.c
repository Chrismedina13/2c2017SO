#include "serializacion.h"
#include "estructuras.h"
#include "commons/string.h"

char* serializeInt(int value){

	size_t size = sizeof(int);
	char* stream = calloc(1,size);
	memcpy(stream,&value,size);
	return stream;
}



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

	//logInfo("Entró a la serializar una estructura\n");
	char* rtaSerializada = malloc(
			sizeof(int) * 6 + strlen(ipWorker)
					+ strlen(archivoTemporal));

	int tamanioIpWorker = strlen(ipWorker);
	int tamanioArchivoTemporal = strlen(archivoTemporal);
	int desplazamiento = 0;

	serializarDato(rtaSerializada, &(tamanioIpWorker), sizeof(int), &desplazamiento);
		//logInfo("Serializo tamanio IpWorker");

	serializarDato(rtaSerializada, &(tamanioArchivoTemporal), sizeof(int), &desplazamiento);
		//logInfo("Serializo tamanio del archivo temporal");

	serializarDato(rtaSerializada, &(nodo), sizeof(int), &desplazamiento);
	//logInfo("Serializo nodo");
	serializarDato(rtaSerializada, &(puertoWorker), sizeof(int),
			&desplazamiento);
	serializarDato(rtaSerializada, &(bloque), sizeof(int), &desplazamiento);
	//logInfo("Serializo bloque");
	serializarDato(rtaSerializada, &(bytesOcupados), sizeof(int),
			&desplazamiento);
	//logInfo("Serializo bytes");
	serializarDato(rtaSerializada, ipWorker, strlen(ipWorker),
			&desplazamiento);
	//logInfo("Serializo ip");
	serializarDato(rtaSerializada, archivoTemporal,
			strlen(archivoTemporal), &desplazamiento);
	//logInfo("Serializo archivo");

	//logInfo("Serializar respuesta transf yama bien");

	return (rtaSerializada);

}

/*DESERIALIZAR DE UN NODO/ESTRUCTURA FUNCIONANDO OK */
RespuestaTransformacionYAMA *deserializarRespuestaTransformacionYAMA(
		char* rtaSerializada) {

	//logInfo("Entro a deserializar la estructura");
	int desplazamiento = 0;
	int tamanioIpWorker = malloc(sizeof(int));
	int tamanioArchivoTemporal = malloc(sizeof(int));


	deserializarDato(&(tamanioIpWorker),rtaSerializada, sizeof(int), &desplazamiento);
	//logInfo("Entro a 1\nTamanio ip worker: %d", tamanioIpWorker);

	deserializarDato(&(tamanioArchivoTemporal),	rtaSerializada, sizeof(int), &desplazamiento);
		///logInfo("Entro a 2\nTamanio archivo temporal: %d", tamanioArchivoTemporal);

	RespuestaTransformacionYAMA * respuestaTransformacionYAMA = malloc((sizeof(int)*4) + tamanioIpWorker + tamanioArchivoTemporal);

	deserializarDato(&(respuestaTransformacionYAMA->nodo),rtaSerializada, sizeof(int), &desplazamiento);
	//logInfo("Entro a 3\nID NODO: %d",respuestaTransformacionYAMA->nodo);

	deserializarDato(&(respuestaTransformacionYAMA->puertoWorker),rtaSerializada, sizeof(int), &desplazamiento);
	//logInfo("Entro a 4\nPuerto: %d",respuestaTransformacionYAMA->puertoWorker);

	deserializarDato(&(respuestaTransformacionYAMA->bloque), rtaSerializada,sizeof(int), &desplazamiento);
	logInfo("Bloque: %d",respuestaTransformacionYAMA->bloque);

	deserializarDato(&(respuestaTransformacionYAMA->bytesOcupados),
			rtaSerializada, sizeof(int), &desplazamiento);
	//logInfo("Entro a 6\nBytesocupados: %d",respuestaTransformacionYAMA->bytesOcupados);

	respuestaTransformacionYAMA->ipWorkwer = string_substring(rtaSerializada,desplazamiento,tamanioIpWorker);
	respuestaTransformacionYAMA->ipWorkwer[tamanioIpWorker +1] = "\0";


	//logInfo("Entro a 7\nIP: %s",respuestaTransformacionYAMA->ipWorkwer);
	//logInfo("LLego ip WORKER");
	desplazamiento+= tamanioIpWorker;

	respuestaTransformacionYAMA->archivoTemporal = string_substring(rtaSerializada,desplazamiento,tamanioArchivoTemporal);
	respuestaTransformacionYAMA->archivoTemporal[tamanioArchivoTemporal +1] = "\0";

	//logInfo("Entro a 6\nARchivo: %s",respuestaTransformacionYAMA->archivoTemporal);
	desplazamiento+= tamanioArchivoTemporal;


	//logInfo("LLego a archivo");

	//logInfo("Respuesta archivo temporal: %s", respuestaTransformacionYAMA->archivoTemporal);
	//logInfo("DESERIALIZAR COMPLETO");


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


char* serializarInfoParaWorker(int nodo, int bloque, int bytesOcupados,
		char* archivoTemporal) {

	logInfo("Entró a la serializar una estructua INFOPARAWORKER\n");
	int tamanioArchivoTemporal = strlen(archivoTemporal);

	logInfo("Se obtuvo: tamanioArchivoTemp: %i\n",tamanioArchivoTemporal);

	char* rtaSerializada = malloc(sizeof(int) * 4 + tamanioArchivoTemporal);
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

infoTransformacionParaWorker *deserializarInfoParaWorker(char* rtaSerializada) {

	logInfo("ENTRO A DESERIALIZAR INFOPARAWORKER");
	int desplazamiento = 0;
	int tamanioArchivoTemp =malloc(sizeof(int));

	deserializarDato(&(tamanioArchivoTemp), rtaSerializada, sizeof(int),&desplazamiento);

	infoTransformacionParaWorker * respuesta = malloc(tamanioArchivoTemp+sizeof(int) * 3);

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

char * serializarLista_info_workers(t_list * listaWorkers) {

	uint32_t total_size = tamanioEstructurasListaWorkers(listaWorkers);
	char *listaSerializada = malloc((sizeof(char)*total_size) + ((list_size(listaWorkers)*sizeof(uint32_t))*2)+ sizeof(int));
	int offset = 0;
	int i;
	int tamanio_lista = list_size(listaWorkers);
	serializarDato(listaSerializada,&(tamanio_lista),sizeof(int),&offset);
	for (i = 0; i < list_size(listaWorkers); i++) {
		Info_Workers* info = list_get(listaWorkers,i);
		int tamanioInfoWorker = tamanioEstructuraInfoWorker(info);
		int tamanioIP = strlen(info->ipWorker);
		serializarDato(listaSerializada,&(tamanioInfoWorker),sizeof(int),&offset);
		serializarDato(listaSerializada,&(info->puerto),sizeof(int),&offset);
		serializarDato(listaSerializada,&(tamanioIP),sizeof(int),&offset);
		serializarDato(listaSerializada,info->ipWorker,tamanioIP,&offset);
	}

	return listaSerializada;
}



t_list * deserializarLista_info_workers(char * listaWorkersSerializada) {
		int tamanio_lista;

		int desplazamiento = 0;
		t_list* lista = list_create();
		int i;

		deserializarDato(&(tamanio_lista),listaWorkersSerializada,sizeof(int),&desplazamiento);

		for (i = 0; i < tamanio_lista; i++) {

			int tamanioInfoWorker;
			deserializarDato(&(tamanioInfoWorker),listaWorkersSerializada,sizeof(int),&desplazamiento);
			Info_Workers* info = malloc(tamanioInfoWorker);
			deserializarDato(&(info->puerto),listaWorkersSerializada,sizeof(int),&desplazamiento);
			int tamanioIP ;
			deserializarDato(&(tamanioIP),listaWorkersSerializada,sizeof(int),&desplazamiento);
			info->ipWorker = string_substring(listaWorkersSerializada,desplazamiento,tamanioIP);
			desplazamiento += tamanioIP;
			list_add(lista,info);

		}
		return lista;
}


char* serializarInfoWorker(int puerto, char* ipWorker) {
	char* infoWorkerSerializado = malloc(
			strlen(ipWorker) + sizeof(char) + sizeof(int));
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

	int tamanioIP = sizeof(malloc(sizeof(int)));
	int desplazamiento = 0;

	deserializarDato(&(tamanioIP), infoWorkerSerializado, sizeof(int),
				&desplazamiento);

	Info_Workers *infoworker = malloc(tamanioIP + sizeof(int));

	deserializarDato(&(infoworker->puerto), infoWorkerSerializado, sizeof(int),
			&desplazamiento);
	infoworker->ipWorker = string_substring(infoWorkerSerializado,desplazamiento,tamanioIP);
	desplazamiento+=tamanioIP;

	return infoworker;

}


char * serializarUblicacionBloqueArchivo(UbicacionBloquesArchivo * estructura) {
	char* estructuraSerializada = malloc(sizeof(int) * 6);
	int desplazamiento = 0;

	serializarDato(estructuraSerializada, &(estructura->parteDelArchivo), sizeof(int), &desplazamiento);
	serializarDato(estructuraSerializada, &(estructura->bytesOcupados),	sizeof(int), &desplazamiento);
	char* ubicacion1serializada = sizeof(int)*2;
	ubicacion1serializada =serializarUbicacionBloque(estructura->ubicacionCopia1);
	char* ubicacion2serializada = sizeof(int)*2;
		ubicacion2serializada = serializarUbicacionBloque(estructura->ubicacionCopia2);
	serializarDato(estructuraSerializada,ubicacion1serializada,sizeof(int)*2, &desplazamiento);
	serializarDato(estructuraSerializada,ubicacion2serializada,sizeof(int)*2, &desplazamiento);

	return estructuraSerializada;
}

UbicacionBloquesArchivo *deserializarUbicacionArchivo(char* UbicacionSerializada) {
	UbicacionBloquesArchivo * ubicacionbloquesarchivos = malloc(sizeof(int) * 6);
	int desplazamientoF = 0;

	deserializarDato(&(ubicacionbloquesarchivos->parteDelArchivo),UbicacionSerializada, sizeof(int), &desplazamientoF);
	deserializarDato(&(ubicacionbloquesarchivos->bytesOcupados),UbicacionSerializada, sizeof(int), &desplazamientoF);
	deserializarDato(&(ubicacionbloquesarchivos->ubicacionCopia1.nodo),UbicacionSerializada, sizeof(int), &desplazamientoF);
	deserializarDato(&(ubicacionbloquesarchivos->ubicacionCopia1.desplazamiento),UbicacionSerializada, sizeof(int), &desplazamientoF);
	deserializarDato(&(ubicacionbloquesarchivos->ubicacionCopia2.nodo),UbicacionSerializada, sizeof(int), &desplazamientoF);
	deserializarDato(&(ubicacionbloquesarchivos->ubicacionCopia2.desplazamiento),UbicacionSerializada, sizeof(int), &desplazamientoF);

	return ubicacionbloquesarchivos;
}

char* serializarUbicacionBloque(UbicacionBloque ubicacionbloque) {
	char* ubicacionBloqueSerializado = malloc(sizeof(int) * 2);
	int desplazamiento = 0;
	serializarDato(ubicacionBloqueSerializado, &(ubicacionbloque.nodo),	sizeof(int), &desplazamiento);
	serializarDato(ubicacionBloqueSerializado,&(ubicacionbloque.desplazamiento), sizeof(int), &desplazamiento);
	return ubicacionBloqueSerializado;
}

UbicacionBloque deserializarUbicacionBloque(char* ubicacionbloqueserializado) {
	UbicacionBloque ubicacionbloque;
	int desplazamiento = 0;
	deserializarDato(&(ubicacionbloque.nodo), ubicacionbloqueserializado,sizeof(int), &desplazamiento);
	deserializarDato(&(ubicacionbloque.desplazamiento),	ubicacionbloqueserializado, sizeof(int), &desplazamiento);

	return ubicacionbloque;

}

char* serializarRespuestaReduccionLocal(RespuestaReduccionLocal* info){
	logInfo("inicio Serializo redu local");

	int tamanioArchivoRedu=strlen(info->archivoReduccionLocal);
	int tamanioip=strlen(info->ipWorker);
	int tamanioLista=tamanioListaDeArchivos(info->archivosDeTransformacion);
	int cantidadElementosLista = list_size(info->archivosDeTransformacion);

	char* infoSerializada = malloc(tamanioArchivoRedu+tamanioip+tamanioLista+sizeof(int)*6 + sizeof(int)*cantidadElementosLista);
	int offset = 0;

	serializarDato(infoSerializada,&(tamanioArchivoRedu),sizeof(int),&offset);
	serializarDato(infoSerializada,&(tamanioip),sizeof(int),&offset);
	serializarDato(infoSerializada,&(tamanioLista),sizeof(int),&offset);
	serializarDato(infoSerializada,&(cantidadElementosLista),sizeof(int),&offset);

	serializarDato(infoSerializada,&(info->nodo),sizeof(int),&offset);
	serializarDato(infoSerializada,&(info->puertoWorker),sizeof(int),&offset);

	serializarDato(infoSerializada,info->archivoReduccionLocal,tamanioArchivoRedu,&offset);
	serializarDato(infoSerializada,info->ipWorker,tamanioip,&offset);

	int i = 0;
	while(i<cantidadElementosLista){
		char* contenido = list_get(info->archivosDeTransformacion,i);
		int tamanioContenido = strlen(contenido);
		serializarDato(infoSerializada,&(tamanioContenido),sizeof(int),&offset);
		serializarDato(infoSerializada,contenido,tamanioContenido,&offset);
		i++;
	}
	logInfo(" termino Serializo redu local");
	return infoSerializada;
}
RespuestaReduccionLocal* deserializarRespuestaReduccionLocal(char* infoSerializada){
	int tamanioArchivoRedu;
	int tamanioLista;
	int tamanioip ;
	int cantidadElementosLista ;
	int tamanioContenido;
	int offset = 0;
	logInfo("deserializo redu local");


	deserializarDato(&(tamanioArchivoRedu),infoSerializada,sizeof(int),&offset);
	deserializarDato(&(tamanioip),infoSerializada,sizeof(int),&offset);
	deserializarDato(&(tamanioLista),infoSerializada,sizeof(int),&offset);
	deserializarDato(&(cantidadElementosLista),infoSerializada,sizeof(int),&offset);

	RespuestaReduccionLocal* info = malloc(tamanioArchivoRedu+ tamanioip+ tamanioLista + sizeof(int)*2);
	deserializarDato(&(info->nodo),infoSerializada,sizeof(int),&offset);
	deserializarDato(&(info->puertoWorker),infoSerializada,sizeof(int),&offset);
	logInfo("%i",info->nodo);
	logInfo("%i",info->puertoWorker);
	info->archivoReduccionLocal=string_substring(infoSerializada,offset,tamanioArchivoRedu);
	offset+=tamanioArchivoRedu;
	info->ipWorker=string_substring(infoSerializada,offset,tamanioip);
		offset+=tamanioip;
		logInfo("%s",info->archivoReduccionLocal);
		logInfo("%s",info->ipWorker);
int i;
	logInfo("%i",cantidadElementosLista);
	t_list* lista = list_create();
	for (i = 0; i < cantidadElementosLista; i++) {

		deserializarDato(&(tamanioContenido),infoSerializada,sizeof(int),&offset);
		char* contenido = malloc(tamanioContenido);
		contenido = string_substring(infoSerializada,offset,tamanioContenido);
		offset += tamanioContenido;
		logInfo("%s",contenido);
		list_add(lista,contenido);
		info->archivosDeTransformacion = lista;
	}

logInfo("Deserilizo fin");
	return info;

}
char* serializarResultado(resultadoJob* fin) {

	int desplazamiento = 0;
	char* finTransformcaionSerializado = malloc(sizeof(int) * 2);

	serializarDato(finTransformcaionSerializado, &(fin->nodo), sizeof(int),	&desplazamiento);
	serializarDato(finTransformcaionSerializado, &(fin->resultado),	sizeof(int), &desplazamiento);

	return (finTransformcaionSerializado);
}

resultadoJob * deserializarResultado(char* FT) {

	int desplazamiento = 0;
	resultadoJob* fin = malloc(sizeof(int) * 2);
	deserializarDato(&(fin->nodo),FT, sizeof(int), &desplazamiento);
	deserializarDato(&(fin->resultado),FT, sizeof(int), &desplazamiento);
	return fin;

}

char* serializarFinReduccionLocal(finReduccionLocal* fin) {

	int desplazamiento = 0;
	char* finRLSerializado = malloc(sizeof(int) * 3);
	serializarDato(finRLSerializado, &(fin->nodo), sizeof(int),&desplazamiento);
	serializarDato(finRLSerializado, &(fin->numeroDeJob),sizeof(int), &desplazamiento);
	serializarDato(finRLSerializado, &(fin->resultado),sizeof(int), &desplazamiento);

	return (finRLSerializado);
}

finReduccionLocal * deserializarFinReduccionLocal(char* FT) {

	int desplazamiento = 0;
	finReduccionLocal* fin = malloc(sizeof(int) * 3);
	deserializarDato(&(fin->nodo),FT,sizeof(int), &desplazamiento);
	deserializarDato(&(fin->numeroDeJob),FT, sizeof(int), &desplazamiento);
	deserializarDato(&(fin->resultado),FT, sizeof(int), &desplazamiento);

	return fin;

}

char* serializarFinTransformacion(finTransformacion* fin) {

	int desplazamiento = 0;
	char* finTransformcaionSerializado = malloc(sizeof(int) * 4);
	serializarDato(finTransformcaionSerializado, &(fin->nodo), sizeof(int),&desplazamiento);
	serializarDato(finTransformcaionSerializado, &(fin->numeroDeJob),sizeof(int), &desplazamiento);
	serializarDato(finTransformcaionSerializado, &(fin->bloque),sizeof(int), &desplazamiento);
	serializarDato(finTransformcaionSerializado, &(fin->resultado),sizeof(int), &desplazamiento);

	return (finTransformcaionSerializado);
}

finTransformacion * deserializarFinTransformacion(char* FT) {

	int desplazamiento = 0;
	finTransformacion* fin = malloc(sizeof(int) * 4);
	deserializarDato(&(fin->nodo),FT,sizeof(int), &desplazamiento);
	deserializarDato(&(fin->numeroDeJob),FT, sizeof(int), &desplazamiento);
	deserializarDato(&(fin->bloque),FT, sizeof(int), &desplazamiento);
	deserializarDato(&(fin->resultado),FT, sizeof(int), &desplazamiento);

	return fin;

}
int deserializarINT(char* stream) {
	size_t size = sizeof(int);
	int value;
	memcpy(&value, stream, size);
	return value;
}
/* Serializacion y deserializacion de una lista de la nueva ubicacion bloques archivos */

char* serializarListaUbicacionBloquesArchivo2(t_list* listaDeUbicacionesDeBloquesArchivos){

	int desplazamiento = 0;
	int tamanioLista = list_size(listaDeUbicacionesDeBloquesArchivos);
	int i = 0;
	char* listaSerializada = malloc(sizeof(UbicacionBloquesArchivo2)*list_size(listaDeUbicacionesDeBloquesArchivos));

	serializarDato(listaSerializada,&(tamanioLista),sizeof(int),&desplazamiento);

	while(i < tamanioLista){

		UbicacionBloquesArchivo2* ubi = list_get(listaDeUbicacionesDeBloquesArchivos,i);

		serializarDato(listaSerializada,&(ubi->bytesOcupados),sizeof(int),&desplazamiento);
		serializarDato(listaSerializada,&(ubi->desplazamiento1),sizeof(int),&desplazamiento);
		serializarDato(listaSerializada,&(ubi->desplazamiento2),sizeof(int),&desplazamiento);
		serializarDato(listaSerializada,&(ubi->nodo1),sizeof(int),&desplazamiento);
		serializarDato(listaSerializada,&(ubi->nodo2),sizeof(int),&desplazamiento);
		serializarDato(listaSerializada,&(ubi->parteDelArchivo),sizeof(int),&desplazamiento);

		i++;
	}

	return listaSerializada;
}

UbicacionBloquesArchivo2* deserializarListaUbicacionBloquesArchivo2(char* listaSerilizada){

	int desplazamiento = 0;
	int tamanioLista;
	int i = 0;
	t_list* listaUbicacionBloquesArchivos2 = list_create();

	deserializarDato(&(tamanioLista),listaSerilizada,sizeof(int),&desplazamiento);

	while(i < tamanioLista){

		UbicacionBloquesArchivo2* ubi = malloc(sizeof(int)*6);

		deserializarDato(&(ubi->bytesOcupados),listaSerilizada,sizeof(int),&desplazamiento);
		deserializarDato(&(ubi->desplazamiento1),listaSerilizada,sizeof(int),&desplazamiento);
		deserializarDato(&(ubi->desplazamiento2),listaSerilizada,sizeof(int),&desplazamiento);
		deserializarDato(&(ubi->nodo1),listaSerilizada,sizeof(int),&desplazamiento);
		deserializarDato(&(ubi->nodo2),listaSerilizada,sizeof(int),&desplazamiento);
		deserializarDato(&(ubi->parteDelArchivo),listaSerilizada,sizeof(int),&desplazamiento);

		list_add(listaUbicacionBloquesArchivos2,ubi);
		i++;
	}
	logInfo("Terminando la deserializacion deserializarListaUbicacionBloquesArchivo2");
	return listaUbicacionBloquesArchivos2;
}



////////////////////////////////////////////////////////////////////////////////////////





char* serializarContenidoDelBloque(char* contenidoBloque){

	char *bloqueSerializado = malloc(strlen(contenidoBloque) + sizeof(int));
	int desplazamiento = 0;
	int tamanio = strlen(contenidoBloque);
	//int TamanioDataSetBloque = strlen(contenidoBloque) + sizeof(int) ;

	//serializarDato(bloqueSerializado, &(TamanioDataSetBloque), sizeof(int), &desplazamiento);

	//serializarDato(bloqueSerializado, &(numeroBloque), sizeof(int),&desplazamiento);

	serializarDato(bloqueSerializado, &(tamanio), sizeof(int),&desplazamiento);

	serializarDato(bloqueSerializado, contenidoBloque,tamanio, &desplazamiento);

	return bloqueSerializado;
}


char* serializarBloque(int numeroBloque, char* contenidoBloque) {

	char *bloqueSerializado = malloc(strlen(contenidoBloque) + sizeof(int)*3);
	int desplazamiento = 0;
	int tamanio = strlen(contenidoBloque);
	int TamanioDataSetBloque = strlen(contenidoBloque) + sizeof(int) ;

	serializarDato(bloqueSerializado, &(TamanioDataSetBloque), sizeof(int), &desplazamiento);

	serializarDato(bloqueSerializado, &(numeroBloque), sizeof(int),&desplazamiento);

	serializarDato(bloqueSerializado, &(tamanio), sizeof(int),&desplazamiento);

	serializarDato(bloqueSerializado, contenidoBloque,tamanio, &desplazamiento);

	return bloqueSerializado;
}

char* deserilizarContenidoBloque(char* bloqueSerializado) {

	int desplazamiento = 0;
	int tamanioContenido;
	int tamanioDelSetBloque;
	deserializarDato(&(tamanioDelSetBloque), bloqueSerializado, sizeof(int),&desplazamiento);

	char* contenidoBloque = malloc(tamanioDelSetBloque);

	contenidoBloque = string_substring(bloqueSerializado,desplazamiento,tamanioDelSetBloque);

	return contenidoBloque;
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

//Respuesta ALmacenado Final

char* serializarRespuestaAlmacenadoFinal(respuestaAlmacenadoFinal* RAF){

	char* RAFSerializado = malloc(sizeof(int)*5 + strlen(RAF->archivoDeReduccionGlobal) + strlen(RAF->ipWorker));
	int desplazamiento = 0;
	int TamanioRAF = (sizeof(int)*2 + strlen(RAF->archivoDeReduccionGlobal) + strlen(RAF->ipWorker));

	serializarDato(RAFSerializado, &(TamanioRAF), sizeof(int),&desplazamiento);
	serializarDato(RAFSerializado, &(RAF->nodo), sizeof(int),&desplazamiento);
	serializarDato(RAFSerializado, &(RAF->puertoWorker), sizeof(int),&desplazamiento);
	int tamanioArchivoReduccionglobal = strlen(RAF->archivoDeReduccionGlobal);
	serializarDato(RAFSerializado, &(tamanioArchivoReduccionglobal), sizeof(int),&desplazamiento);
	serializarDato(RAFSerializado, RAF->archivoDeReduccionGlobal, tamanioArchivoReduccionglobal,&desplazamiento);
	int tamanioIP = strlen(RAF->ipWorker);
	serializarDato(RAFSerializado, &(tamanioIP), sizeof(int),&desplazamiento);
	serializarDato(RAFSerializado, RAF->ipWorker, tamanioIP,&desplazamiento);

	return RAFSerializado;


}


respuestaAlmacenadoFinal* deserializarRespuestaAlmacenadoFinal(char* almacenadoFinalSerializado) {
	int desplazamiento = 0;
	int tamanioRespuesta;

	deserializarDato(&(tamanioRespuesta), almacenadoFinalSerializado, sizeof(int),&desplazamiento);
	respuestaAlmacenadoFinal* RAF = malloc(tamanioRespuesta);

	deserializarDato(&(RAF->nodo), almacenadoFinalSerializado, sizeof(int),&desplazamiento);
	deserializarDato(&(RAF->puertoWorker), almacenadoFinalSerializado, sizeof(int),&desplazamiento);
	int tamanioArchivo;
	deserializarDato(&(tamanioArchivo), almacenadoFinalSerializado, sizeof(int),&desplazamiento);
	RAF->archivoDeReduccionGlobal = string_substring(almacenadoFinalSerializado,desplazamiento,tamanioArchivo);
	desplazamiento += tamanioArchivo;

	int tamanioIP;
	deserializarDato(&(tamanioIP), almacenadoFinalSerializado, sizeof(int),&desplazamiento);
	RAF->ipWorker = string_substring(almacenadoFinalSerializado,desplazamiento,tamanioIP);
	desplazamiento += tamanioIP;

	return RAF;

}

//Respuesta Reduccion Global
int tamanioListareduccionGlobal(t_list* listaRRG){

	int i = 0;
	int tamanio = 0;

	while(i< list_size(listaRRG)){
		RespuestaReduccionGlobal* RRG = list_get(listaRRG,i);
		tamanio += sizeof(int)*2 + sizeof(bool);
		tamanio += strlen(RRG->ipWorker);
		tamanio += strlen(RRG->archivoReduccionGlobal);
		tamanio += strlen(RRG->archivoReduccionLocal);
		i++;
	}
	return tamanio;
}


char* serializarListaRespuestaReduccionGlobal(t_list* listaRRG){

	char *listaSerializada = malloc(tamanioListareduccionGlobal(listaRRG) + sizeof(int) + (sizeof(int)*list_size(listaRRG)*4) );
	int offset = 0;
	int i;
	int tamanio_lista = list_size(listaRRG);

	serializarDato(listaSerializada,&(tamanio_lista),sizeof(int),&offset);

	for (i = 0; i < list_size(listaRRG); i++){

		RespuestaReduccionGlobal* nodo = list_get(listaRRG,i);
		int tamanioDeNodo = sizeof(int)*2 + sizeof(bool) + strlen(nodo->archivoReduccionGlobal) + strlen(nodo->archivoReduccionLocal) + strlen(nodo->ipWorker) ;
		serializarDato(listaSerializada,&(tamanioDeNodo),sizeof(int),&offset);

		serializarDato(listaSerializada,&(nodo->nodo),sizeof(int),&offset);
		serializarDato(listaSerializada,&(nodo->puertoWorker),sizeof(int),&offset);
		serializarDato(listaSerializada,&(nodo->encargado),sizeof(bool),&offset);

		int tamanioArchivoReduccionLocal = strlen(nodo->archivoReduccionLocal);
		serializarDato(listaSerializada,&(tamanioArchivoReduccionLocal),sizeof(int),&offset);
		serializarDato(listaSerializada,nodo->archivoReduccionLocal,tamanioArchivoReduccionLocal,&offset);

		int tamanioArchivoReduccionGlobal = strlen(nodo->archivoReduccionGlobal);
		serializarDato(listaSerializada,&(tamanioArchivoReduccionGlobal),sizeof(int),&offset);
		serializarDato(listaSerializada,nodo->archivoReduccionGlobal,tamanioArchivoReduccionGlobal,&offset);


		int tamanioIP = strlen(nodo->ipWorker);
		serializarDato(listaSerializada,&(tamanioIP),sizeof(int),&offset);
		serializarDato(listaSerializada,nodo->ipWorker,tamanioIP,&offset);
	}
	return listaSerializada;
}

t_list * deserializarListaRespuesReduccionGlobal(char* ListaRRGSerializada){

	int tamanio_lista;
	int desplazamiento = 0;
	t_list* lista = list_create();
	int i;

	deserializarDato(&(tamanio_lista),ListaRRGSerializada,sizeof(int),&desplazamiento);

	for (i = 0; i < tamanio_lista; i++) {

		int tamanioDelNodo;
		deserializarDato(&(tamanioDelNodo),ListaRRGSerializada,sizeof(int),&desplazamiento);
		RespuestaReduccionGlobal* RRG = malloc(tamanioDelNodo);

		deserializarDato(&(RRG->nodo),ListaRRGSerializada,sizeof(int),&desplazamiento);
		deserializarDato(&(RRG->puertoWorker),ListaRRGSerializada,sizeof(int),&desplazamiento);
		deserializarDato(&(RRG->encargado),ListaRRGSerializada,sizeof(bool),&desplazamiento);

		int tamanioArchivoLocal;
		deserializarDato(&(tamanioArchivoLocal),ListaRRGSerializada,sizeof(int),&desplazamiento);
		RRG->archivoReduccionLocal = string_substring(ListaRRGSerializada,desplazamiento,tamanioArchivoLocal);
		desplazamiento += tamanioArchivoLocal;

		int tamanioArchivoGlobal;
		deserializarDato(&(tamanioArchivoGlobal),ListaRRGSerializada,sizeof(int),&desplazamiento);
		RRG->archivoReduccionGlobal = string_substring(ListaRRGSerializada,desplazamiento,tamanioArchivoGlobal);
		desplazamiento += tamanioArchivoGlobal;

		int tamanioIP;
		deserializarDato(&(tamanioIP),ListaRRGSerializada,sizeof(int),&desplazamiento);
		RRG->ipWorker = string_substring(ListaRRGSerializada,desplazamiento,tamanioIP);
		desplazamiento += tamanioIP;

		list_add(lista,RRG);

	}
	return lista;

}

char* serializarInfoParaReduccionGlobal(infoParaReduccionGlobal* info){
	int tamanioIpWorker = strlen(info->ipWorker);
	int tamanioArchivo =strlen(info->archivoTemporalReduccionLocal);
	char* infoSerializada = malloc(tamanioIpWorker+sizeof(int)+tamanioArchivo);
	int offset = 0;
	char* ip = malloc(strlen(info->ipWorker));
	char* archivo = malloc(strlen(info->archivoTemporalReduccionLocal));
	ip = info->ipWorker;
	archivo = info->archivoTemporalReduccionLocal;
	serializarDato(infoSerializada,&(tamanioArchivo),sizeof(int),&offset);
	serializarDato(infoSerializada,&(tamanioIpWorker),sizeof(int),&offset);
	serializarDato(infoSerializada,&(info->puerto),sizeof(int),&offset);
	serializarDato(infoSerializada,(archivo),tamanioArchivo,&offset);
	serializarDato(infoSerializada,(ip),tamanioIpWorker,&offset);

	return infoSerializada;
}

infoParaReduccionGlobal* deserializarInfoParaReduccionGlobal(char* infoSerializada){
	int tamanioIpWorker;
	int tamanioArchivo;
	int offset = 0;
	deserializarDato(&(tamanioArchivo),infoSerializada,sizeof(int),&offset);
	deserializarDato(&(tamanioIpWorker),infoSerializada,sizeof(int),&offset);
	infoParaReduccionGlobal* infoDeserializada = malloc(sizeof(int)+tamanioArchivo + tamanioIpWorker);
	deserializarDato(&(infoDeserializada->puerto),infoSerializada,sizeof(int),&offset);
	infoDeserializada->archivoTemporalReduccionLocal = string_substring(infoSerializada,offset,tamanioArchivo);
	offset+=tamanioArchivo;
	infoDeserializada->ipWorker = string_substring(infoSerializada,offset,tamanioIpWorker);

	return infoDeserializada;
}


char* serializarInfoReduccionGlobalDeMasterParaWorker(infoReduccionGlobalDeMasterParaWorker* info){
	int tamanioArchivoTemporal=strlen(info->archivoTemporalReduccionGlobal);
	int tamanioLista=tamanioListaDeArchivos(info->listaInfoParaReduccionGlobal);
	int tamanioNombreScript=strlen(info->scriptReduccionGlobal.nombre);
	int tamanioContenidoScript=strlen(info->scriptReduccionGlobal.contenido);
	int cantidadElementosLista = list_size(info->listaInfoParaReduccionGlobal);


	char* infoSerializada = malloc(tamanioArchivoTemporal+tamanioLista+tamanioNombreScript+tamanioContenidoScript+sizeof(int)*5 + sizeof(int)*cantidadElementosLista);
	int offset = 0;


	serializarDato(infoSerializada,&(tamanioArchivoTemporal),sizeof(int),&offset);
	serializarDato(infoSerializada,&(tamanioNombreScript),sizeof(int),&offset);
	serializarDato(infoSerializada,&(tamanioContenidoScript),sizeof(int),&offset);
	serializarDato(infoSerializada,&(tamanioLista),sizeof(int),&offset);
	serializarDato(infoSerializada,&(cantidadElementosLista),sizeof(int),&offset);

	serializarDato(infoSerializada,info->archivoTemporalReduccionGlobal,tamanioArchivoTemporal,&offset);
	serializarDato(infoSerializada,info->scriptReduccionGlobal.nombre,tamanioNombreScript,&offset);
	serializarDato(infoSerializada,info->scriptReduccionGlobal.contenido,tamanioContenidoScript,&offset);

	int i = 0;
	while(i<cantidadElementosLista){
		char* contenido = list_get(info->listaInfoParaReduccionGlobal,i);
		int tamanioContenido = strlen(contenido);
		serializarDato(infoSerializada,&(tamanioContenido),sizeof(int),&offset);
		serializarDato(infoSerializada,contenido,tamanioContenido,&offset);
		i++;
	}

	return infoSerializada;
}

infoReduccionGlobalDeMasterParaWorker* deserializarInfoReduccionGlobalDeMasterParaWorker(char* infoSerializada){
	int tamanioArchivoTemporal;
	int tamanioLista;
	int tamanioNombreScript;
	int tamanioContenidoScript;
	int cantidadElementosLista;
	int tamanioContenido;
	int offset = 0;

	deserializarDato(&(tamanioArchivoTemporal),infoSerializada,sizeof(int),&offset);
	deserializarDato(&(tamanioNombreScript),infoSerializada,sizeof(int),&offset);
	deserializarDato(&(tamanioContenidoScript),infoSerializada,sizeof(int),&offset);
	deserializarDato(&(tamanioLista),infoSerializada,sizeof(int),&offset);
	deserializarDato(&(cantidadElementosLista),infoSerializada,sizeof(int),&offset);

	infoReduccionGlobalDeMasterParaWorker* info = malloc(tamanioArchivoTemporal+tamanioLista+tamanioNombreScript+tamanioContenidoScript);
	info->archivoTemporalReduccionGlobal=string_substring(infoSerializada,offset,tamanioArchivoTemporal);
	offset+=tamanioArchivoTemporal;
	char* nombre = string_substring(infoSerializada,offset,tamanioNombreScript);
	info->scriptReduccionGlobal.nombre= nombre;
	offset+=tamanioNombreScript;
	char* contenido = string_substring(infoSerializada,offset,tamanioContenidoScript);
	info->scriptReduccionGlobal.contenido = contenido;
	offset+=tamanioContenidoScript;
	int i=0;
	while(i<cantidadElementosLista){
		deserializarDato(&(tamanioContenido),infoSerializada,sizeof(int),&offset);
		char* contenido = string_substring(infoSerializada,offset,tamanioContenido);
		offset+=tamanioContenido;
		list_add(info->listaInfoParaReduccionGlobal,contenido);
	}

	return info;

}

t_list* deserializarListaArchivos(char* listaSerializada){
	int tamanioDeLaListaDeArchivosTemporales;
	int i = 0;
	int desplazamiento = 0;
	int tamanioDelarchivoTemporal = malloc(sizeof(int));
	t_list* lista = list_create();
	deserializarDato(&(tamanioDeLaListaDeArchivosTemporales),listaSerializada,sizeof(int),&desplazamiento);
	while(i < tamanioDeLaListaDeArchivosTemporales){
		deserializarDato(&(tamanioDelarchivoTemporal),listaSerializada,sizeof(int),&desplazamiento);
		char* archivo = string_substring(listaSerializada,desplazamiento,tamanioDelarchivoTemporal);
		desplazamiento += tamanioDelarchivoTemporal;
		list_add(lista,archivo);
		i++;
	}
	return lista;
}

char * serializarListaArchivos(t_list * lista) {
	int tamanioAchivo = malloc(sizeof(int));
	int tamanioLista = list_size(lista);
	int i;
	char* listaSerializada = malloc(tamanioListaDeArchivos(lista)+sizeof(int)+sizeof(int)*tamanioLista);
	int offset = 0;
	serializarDato(listaSerializada,&(tamanioLista),sizeof(int),&offset);

	for (i = 0; i < list_size(lista); i++) {
		char * archivo = list_get(lista, i);
		tamanioAchivo = strlen(archivo);
		serializarDato(listaSerializada,&(tamanioAchivo),sizeof(int),&offset);
		serializarDato(listaSerializada,archivo,tamanioAchivo,&offset);

	}
	return (listaSerializada);
}

int tamanioinfoReduccionLocalParaWorker(infoReduccionLocalParaWorker* info){

	int tamanio = 0;
	int i = 0;
	while(i < list_size(info->listaDeArchivosTemporales)){

		char* archivo = list_get(info->listaDeArchivosTemporales,i);
		tamanio += strlen(archivo);
		i++;
	}

	tamanio += strlen(info->archivoTemporalReduccionLocal);

	return tamanio;
}



char* serializarinfoReduccionLocalParaWorker(infoReduccionLocalParaWorker* info){

	int desplazamiento = 0;
	char* infoRLPWSerializada = malloc(tamanioinfoReduccionLocalParaWorker(info) + sizeof(int) * list_size(info->listaDeArchivosTemporales) + (sizeof(int)*3));
	int i = 0;

	int tamanioDelinfoReduccionLocalParaWorkerAMandar = tamanioinfoReduccionLocalParaWorker(info);
	serializarDato(infoRLPWSerializada,&(tamanioDelinfoReduccionLocalParaWorkerAMandar),sizeof(int),&desplazamiento);

	int tamanioDeLaListaDeArchivosTemporales = list_size(info->listaDeArchivosTemporales);
	serializarDato(infoRLPWSerializada,&(tamanioDeLaListaDeArchivosTemporales),sizeof(int),&desplazamiento);

	while(i< list_size(info->listaDeArchivosTemporales)){

		char* archivo = list_get(info->listaDeArchivosTemporales,i);
		int tamanioArchivo = strlen(archivo);
		serializarDato(infoRLPWSerializada,&(tamanioArchivo),sizeof(int),&desplazamiento);
		serializarDato(infoRLPWSerializada,archivo,tamanioArchivo,&desplazamiento);
		i++;
	}

	int tamanioArchivoTemporalDeReduccionLocal = strlen(info->archivoTemporalReduccionLocal);
	serializarDato(infoRLPWSerializada,&(tamanioArchivoTemporalDeReduccionLocal),sizeof(int),&desplazamiento);
	serializarDato(infoRLPWSerializada,info->archivoTemporalReduccionLocal,tamanioArchivoTemporalDeReduccionLocal,&desplazamiento);

	return infoRLPWSerializada;
}

infoReduccionLocalParaWorker* deserializarinfoReduccionLocalParaWorker(char* IRLPWSerializado){

	int desplazamiento = 0;
	int i = 0;

	int tamanioDeLaInfo;
	deserializarDato(&(tamanioDeLaInfo),IRLPWSerializado,sizeof(int),&desplazamiento);

	infoReduccionLocalParaWorker* info = malloc(tamanioDeLaInfo);
	info->listaDeArchivosTemporales = list_create();
	int tamanioDeLaListaDeArchivosTemporales;
	deserializarDato(&(tamanioDeLaListaDeArchivosTemporales),IRLPWSerializado,sizeof(int),&desplazamiento);

	while(i < tamanioDeLaListaDeArchivosTemporales){

		int tamanioDelarchivoTemporal;
		deserializarDato(&(tamanioDelarchivoTemporal),IRLPWSerializado,sizeof(int),&desplazamiento);
		char* archivo1 = string_substring(IRLPWSerializado,desplazamiento,tamanioDelarchivoTemporal);
		desplazamiento += tamanioDelarchivoTemporal;
		list_add(info->listaDeArchivosTemporales,archivo1);
		i++;
	}

	int tamanioArchivoTemporalDeReduccionLocal;
	deserializarDato(&(tamanioArchivoTemporalDeReduccionLocal),IRLPWSerializado,sizeof(int),&desplazamiento);
	info->archivoTemporalReduccionLocal = string_substring(IRLPWSerializado,desplazamiento,tamanioArchivoTemporalDeReduccionLocal);
	desplazamiento += tamanioArchivoTemporalDeReduccionLocal;

	return info;
}

char* serializarInfoReduccionLocalDeWorkerParaWorker(infoReduccionGlobalDeWorkerParaWorker* info){

	int desplazamiento = 0;
	char* serializado = malloc(sizeof(int) + strlen(info->archivoTemporalLocalRequerido));

	int Tamanio = strlen(info->archivoTemporalLocalRequerido);
	serializarDato(serializado,&(Tamanio),sizeof(int),&desplazamiento);
	serializarDato(serializado,info->archivoTemporalLocalRequerido, Tamanio,&desplazamiento);

	return serializado;

}

infoReduccionGlobalDeWorkerParaWorker* deserializarinfoReduccionGlobalDeWorkerParaWorker(char* serializado){

	int desplazamiento = 0;
	int tamanioDelMensaje = 0;

	deserializarDato(&(tamanioDelMensaje),serializado,sizeof(int),&desplazamiento);
	infoReduccionGlobalDeWorkerParaWorker* info = malloc(tamanioDelMensaje);

	info->archivoTemporalLocalRequerido = string_substring(serializado,desplazamiento,tamanioDelMensaje);

	return info;
}


char* serializaralmacenadoFinal(almacenadoFinal* AF){

	int desplazamiento = 0;
	char* serializado = malloc(sizeof(int) + strlen(AF->archivoTemporalReduccionGlobal));

	int Tamanio = strlen(AF->archivoTemporalReduccionGlobal);
	serializarDato(serializado,&(Tamanio),sizeof(int),&desplazamiento);
	serializarDato(serializado,AF->archivoTemporalReduccionGlobal, Tamanio,&desplazamiento);

	return serializado;

}

almacenadoFinal* deserializaralmacenadoFinal(char* serializado){

	int desplazamiento = 0;
	int tamanioDelMensaje = 0;

	deserializarDato(&(tamanioDelMensaje),serializado,sizeof(int),&desplazamiento);
	almacenadoFinal* info = malloc(tamanioDelMensaje);

	info->archivoTemporalReduccionGlobal = string_substring(serializado,desplazamiento,tamanioDelMensaje);

	return info;
}


char* serializarReplanificacion(int numeroJob,int nodoCaido){

	char* replanifSerializado = malloc(sizeof(int)*2);

	int desplazamiento = 0;

	serializarDato(replanifSerializado, &(numeroJob), sizeof(int),&desplazamiento);
	serializarDato(replanifSerializado, &(nodoCaido), sizeof(int), &desplazamiento);

	return replanifSerializado;
}

Replanificacion* deserializarReplanificacion(char* FT) {

	int desplazamiento;
	Replanificacion* fin = malloc(sizeof(int) * 2);
	deserializarDato(&(FT), fin->nodoCaido, sizeof(int), &desplazamiento);
	deserializarDato(&(FT), fin->numeroDeJOb, sizeof(int), &desplazamiento);
	return fin;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char* serializarArchivo(archivo* arch) {
	char* nombre = arch->nombre;
	char* contenido = arch->contenido;
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

archivo* deserializarArchivo(char* archivoSerializado) {

	int desplazamiento = 0;
	int tamanioContenido;
	int tamanioNombre;
	deserializarDato(&(tamanioNombre), archivoSerializado,sizeof(int), &desplazamiento);
	deserializarDato(&(tamanioContenido), archivoSerializado,sizeof(int), &desplazamiento);

	archivo* archivoDeserializado = malloc(tamanioContenido+tamanioNombre);

	archivoDeserializado->contenido = string_substring(archivoSerializado,desplazamiento, tamanioContenido);
	desplazamiento = desplazamiento+ tamanioContenido;

	archivoDeserializado->nombre = string_substring(archivoSerializado,desplazamiento,tamanioNombre);
	desplazamiento = desplazamiento+ tamanioNombre;

	free(archivoSerializado);

	return archivoDeserializado;
}
