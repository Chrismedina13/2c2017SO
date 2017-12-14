/*
 * FuncionesAuxiliares.c
 *
 *  Created on: 3/11/2017
 *      Author: utnso
 */

#include "FuncionesAuxiliares.h"



int generarNumeroAleatorioNoRepetido(){

	srand(time(NULL));
	int test = rand() %100;
	printf ("El numero aleatorio del JOB vale %d\n", test);

	return test;

}

char* generarNombreArchivoTransformacion(int variableTransformacion){

	char *unaPalabra = string_new();
	string_append(&unaPalabra, "/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/Worker/tmp/Transf-");
	string_append(&unaPalabra, string_itoa(variableTransformacion));
	return unaPalabra;
}

char* generarNombreArchivoReduccionLocal(int variableRL){

	char *unaPalabra = string_new();
	string_append(&unaPalabra, "/tmp/RL-");
	string_append(&unaPalabra, string_itoa(variableRL));
	return unaPalabra;

}
char* generarNombreArchivoReduccionGlobal(int variableRG){


	char *unaPalabra = string_new();
	string_append(&unaPalabra, "/tmp/RG-");
	string_append(&unaPalabra, string_itoa(variableRG));
	return unaPalabra;
}


char* obtenerPuntero(const char* rutaArchivo){

	struct stat sb;
	off_t len;
	char* p;
	int fd;

	fd = open (rutaArchivo, O_RDONLY);
	//fd = fopen(rutaArchivo,"r");
	if (fd == -1) {
			printf("Error al abrir archivo\n");
			exit(-1);
	}

	if (fstat (fd, &sb) == -1) {
			printf("Error al hacer stat\n");
			exit(-1);
	}

	if (!S_ISREG (sb.st_mode)) {
			printf ("No es un archivo regular\n");
			exit(-1);
	}

	p = (char *)mmap (0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);

	if (p == MAP_FAILED) {
			printf("Fallo el mapeo\n");
			exit(-1);
	}

	if (close (fd) == -1) {
			printf("Error al cerrar el archivo\n");
			exit(-1);
	}

	return p;
}

int tamanioEstructuraInfoWorker(Info_Workers* info){
	int tamanio = sizeof(int) + strlen(info->ipWorker);
	return tamanio;
}
int tamanioEstructurasListaWorkers(t_list* listaWorkers){
	int tamanioWorker;
	int tamanioListaTotal=0;
	int i;
	for(i=0;i<list_size(listaWorkers);i++){
		tamanioWorker = tamanioEstructuraInfoWorker(list_get(listaWorkers,i));
		tamanioListaTotal = tamanioListaTotal+tamanioWorker;
	}
	return tamanioListaTotal;
}
void setInfoWorker(Info_Workers* info, int puerto,char* ipWorker){
	info = malloc(sizeof(int) + strlen(ipWorker) * sizeof(char));
	info->puerto = puerto;
	info->ipWorker = ipWorker;
}

int tamanioEstructuraUbicacionBloquesArchivo(UbicacionBloquesArchivo2* info){
	int tamanio = sizeof(int)*6;
	return tamanio;
}
int tamanioEstructurasListaUbicacionBloquesArchivo(t_list* listaUbicaciones){
	int tamanioUbicacion;
	int tamanioListaTotal=0;
	int i;
	for(i=0;i<list_size(listaUbicaciones);i++){
		tamanioUbicacion = tamanioEstructuraUbicacionBloquesArchivo(list_get(listaUbicaciones,i));
		tamanioListaTotal = tamanioListaTotal+tamanioUbicacion;
	}
	return tamanioListaTotal;
}

script* setScript(char* rutaScript){
	char* punteroAlContenidoDelScript = obtenerPuntero(rutaScript);
	script* script1 = malloc(strlen(rutaScript)+strlen(punteroAlContenidoDelScript));
	script1->nombre = rutaScript;
	script1->contenido = punteroAlContenidoDelScript;

	return script1;
}

void rearmar_script(script* script,int codigo){
	FILE* fd;
	char* nombreArchivo = string_new();
	char* contenidoDelBloque = string_new();
	string_append(&nombreArchivo,"/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/Worker/Scripts/");
	logInfo("Rutaa del script transformacion: %s",nombreArchivo);
	switch(codigo){
	case SCRIPT_REDUCCION:
		string_append(&nombreArchivo,script->nombre);
		rutaScriptReduccion = nombreArchivo; //variable global

		break;
	case SCRIPT_TRANSFORMADOR:
		string_append(&nombreArchivo,script->nombre);
		rutaScriptTransformador = nombreArchivo; //variable global

		break;
	default:
		logInfo("Error codigo incorrecto");
		break;
	}
	fd = fopen(nombreArchivo,"w");
	if (fd==NULL) {
		printf("Error al abrir el archivo.");
	}
	contenidoDelBloque = script->contenido;
	fputs(contenidoDelBloque,fd);
	fclose(fd);
	darPermisosAlLosScriptsPy(nombreArchivo);
}

int ejecutarScriptTransformador(char* rutaScript,char* rutaArchivoAEjecutar,char* rutaArchivoAGuardar){

	char* command = string_new();
	string_append(&command,"/bin/cat ");
	string_append(&command, rutaArchivoAEjecutar);
	string_append(&command, " | ");
	string_append(&command, rutaScript);
	string_append(&command, " | sort > ");
	string_append(&command, rutaArchivoAGuardar);

	logInfo("Comando a ejecutar del script transformador: %s",command);

    int estado = system(command);

    return estado;

}
int ejecutarScriptReductor(char* rutaScript,char* rutaArchivoAEjecutar,char* rutaArchivoAGuardar){

	char* command = string_new();
	string_append(&command,"/bin/cat ");
	string_append(&command, rutaArchivoAEjecutar);
	string_append(&command, " | ");
	string_append(&command, rutaScript);
	string_append(&command, " > ");
	string_append(&command, rutaArchivoAGuardar);

    int estado = system(command);

    return estado;

}
void darPermisosAlLosScriptsPy(char* ruta){
	char* command = string_new();
	string_append(&command,"/bin/chmod 777 ");
	string_append(&command,ruta);
	logInfo("comando a ejecutar: %s",command);
	system(command);
}

void destruirArchivoOScript(char* rutaScript){
	char* command = string_new();
	string_append(&command,"/bin/rm ");
	string_append(&command,rutaScript);
	system(command);
	free(command);
}

int tamanioListaDeArchivos(t_list* lista){
	int tamanio = 0;
	int i = 0;
	while(i<list_size(lista)){
		tamanio += strlen(list_get(lista,i));
		i++;
	}
	return tamanio;
}

int tamanioScript(script script){
	int contenido = strlen(script.contenido);
	int nombre = strlen(script.nombre);

	int tamanioScript = contenido + nombre;
	//logInfo("tamanioScript%i\n",tamanioScript);
	return tamanioScript;
}

// me devuelve la palabra mas chica alfabeticamente
vectorConIndice* LApalabra(vectorConIndice* nodo1,vectorConIndice* nodo2) {
	if(nodo1->vector[nodo1->indice] < nodo2->vector[nodo2->indice]){
		return nodo1;
	}else{
		return nodo2;
	}
}

void apareoDeArchivos(t_list* lista){
	t_list* listaDeContenidos = list_create();
	int cantidadElementosLista = list_size(lista);
	int i = 0;
	char* palabraElegida = string_new();
	//obtengo la lista ocn los contenidos e indices
	while(i<cantidadElementosLista){
		char* contenido = obtenerPuntero(list_get(lista,i));
		char** vectorDeRegistros = string_split(contenido,"\n");
		vectorConIndice* nodo = malloc(sizeof(int)+strlen(contenido));
		nodo->indice = 0;
		nodo->vector = vectorDeRegistros;
		list_add(listaDeContenidos,nodo);
		i++;
	}
	//Creamos el archivo apareado
	FILE* fd;
	fd = fopen("/home/utnso/Escritorio/archivoApareado.txt","w");
	if (fd==NULL) {
		printf("Error al abrir el archivo.");
	}

	//Comparo los contenidos

	int j = i;
	int indicePalabraMasChica;
	int cantidadElementosMenos1;
	char* palabraMasChica;
	vectorConIndice* nodoConPalabraMasChica;
	vectorConIndice* aux;
	while(list_size(listaDeContenidos) > 0){//verifiacmos que la lista siga teniendo elementos
		if(cantidadElementosLista>1){
			j=1;
			nodoConPalabraMasChica = list_get(listaDeContenidos,0);
			indicePalabraMasChica = 0; //pasarle el indice del primer elemento
			palabraMasChica = nodoConPalabraMasChica->vector[nodoConPalabraMasChica->indice];
			//cantidadElementosMenos1 = cantidadElementosLista-1;
			while(j<cantidadElementosLista){
				aux = list_get(listaDeContenidos,j);
				char* palabraAux=aux->vector[aux->indice];
				if(palabraAux < palabraMasChica){
					nodoConPalabraMasChica = aux;
					palabraMasChica = palabraAux;
					indicePalabraMasChica = j;
				}
				j++;

			}
			//mete la palabra en el nuevo archivo y verifia que el indice del nodo no sea el final del archivo
			//y actualizamos la cantidad de elementos lista
			palabraElegida = nodoConPalabraMasChica->vector[nodoConPalabraMasChica->indice];
			fputs(palabraElegida,fd);
			fputs("\n",fd);
			nodoConPalabraMasChica->indice++;
			if(nodoConPalabraMasChica->vector[nodoConPalabraMasChica->indice] == '\0'){
				list_remove(listaDeContenidos,nodoConPalabraMasChica);
				cantidadElementosLista = list_size(listaDeContenidos);	//ACTUALIZO EL TAMAÑO DE LA LISTA DE NODOS
			}

		}
		else{
			//Este es el caso en qe slo me quede un nodo en la lista, hay que meter todo el contenido que le queda en el archivo apareado
			vectorConIndice* aux = list_remove(listaDeContenidos,0);
			while(aux->vector[aux->indice] != '\0'){
				palabraElegida = aux->vector[aux->indice];
				string_append(palabraElegida,"\n");
				fputs(palabraElegida,fd);
				aux->indice++;
			}
		}

	}
	fclose(fd);
}

void apareoDeArchivosVectores(t_list* lista,char* rutaA){
	int cantidadElementosLista = list_size(lista);
	vectorConIndice vectorNodos[cantidadElementosLista];
	int i = 0;
	int nodosSinLeer;
	int datoCompleto;
	int indiceAVerificar;
	char* palabraAux = string_new();
	char* palabraElegida = string_new();
	//obtengo la lista ocn los contenidos e indices
	while(i<cantidadElementosLista){
		char* contenido = obtenerPuntero(list_get(lista,i));
		char** vectorDeRegistros = string_split(contenido,"\n");
		vectorConIndice nodo;
		nodo.indice = 0;
		nodo.vector = vectorDeRegistros;
		nodo.pasoDeDatosCompleto=0;
		vectorNodos[i] = nodo;
		i++;
	}
	//Creamos el archivo apareado
	FILE* fd;
	fd = fopen(rutaA,"w");
	if (fd==NULL) {
		printf("Error al abrir el archivo.");
	}
	//Comparo los contenidos

		int j = i;
		int indicePalabraMasChica;
		int cantidadElementosMenos1;
		char* palabraMasChica;
		vectorConIndice nodoConPalabraMasChica;
		vectorConIndice aux;
		nodosSinLeer =nodosConElementosSinLeer(vectorNodos,cantidadElementosLista);
		while(nodosSinLeer>0){//verifiacmos que la lista siga teniendo elementos
			if(nodosSinLeer>1){
				j=1;
				i=0;
				while(i<cantidadElementosLista){
					datoCompleto = vectorNodos[i].pasoDeDatosCompleto;
					if(vectorNodos[i].pasoDeDatosCompleto == 0){ //si es 0 significa que tiene datos para leer todavia.
						nodoConPalabraMasChica = vectorNodos[i];
						indicePalabraMasChica = i;
						palabraMasChica = nodoConPalabraMasChica.vector[nodoConPalabraMasChica.indice];
						break;
					}
					else i++;
				}
				while(j<cantidadElementosLista){
					if(vectorNodos[j].pasoDeDatosCompleto == 0){ //si es 0 significa que tiene datos para leer todavia.
						aux = vectorNodos[j];
						palabraAux=aux.vector[aux.indice]; //Puntero o estructura?
						palabraElegida = palabraMasChicaEntre(palabraAux,palabraMasChica);
						if(strcmp(palabraElegida,palabraAux) == 0){
							nodoConPalabraMasChica = aux;
							palabraMasChica = palabraAux;
							indicePalabraMasChica = j;
						}
					}
					j++;
				}
				//mete la palabra en el nuevo archivo y verifia que el indice del nodo no sea el final del archivo
				//y actualizamos la cantidad de elementos lista
				palabraElegida = palabraMasChica;
				fputs(palabraElegida,fd);
				fputs("\n",fd);
				indiceAVerificar = vectorNodos[indicePalabraMasChica].indice;
				vectorNodos[indicePalabraMasChica].indice++;
				indiceAVerificar = vectorNodos[indicePalabraMasChica].indice;
				if(vectorNodos[indicePalabraMasChica].vector[vectorNodos[indicePalabraMasChica].indice] == '\0'){
					vectorNodos[indicePalabraMasChica].pasoDeDatosCompleto = 1;
					nodosSinLeer--;
				}
			}
			else{
				//Este es el caso en qe slo me quede un nodo en la lista, hay que meter todo el contenido que le queda en el archivo apareado
				int indice = indiceDelVectorQueQuedanLecturas(vectorNodos,cantidadElementosLista);
				while(vectorNodos[indice].vector[vectorNodos[indice].indice] != '\0'){
					palabraElegida = vectorNodos[indice].vector[vectorNodos[indice].indice];
					fputs(palabraElegida,fd);
					fputs("\n",fd);
					vectorNodos[indice].indice++;
				}
				vectorNodos[indice].pasoDeDatosCompleto = 1;
				nodosSinLeer--;
			}

		}
		fclose(fd);
}

int indiceDelVectorQueQuedanLecturas(vectorConIndice vectorNodos[],int cantidadElementosLista){
	int i = 0;
	int indice = 0;
	while(i<cantidadElementosLista){
		if(vectorNodos[i].pasoDeDatosCompleto == 0){
			indice = i;
			break;
		}
		i++;
	}
	return indice;
}

int nodosConElementosSinLeer(vectorConIndice vectorNodos[], int cantidadElementos){
	int cantidad = 0;
	int i= 0;
	while(i<cantidadElementos){
		if(vectorNodos[i].pasoDeDatosCompleto == 0){
			cantidad++;
		}
		i++;
	}
	return cantidad;
}

char* palabraMasChicaEntre(char* palabraAux1,char* palabraAux2){
	int i = 0;

	while(tolower(palabraAux1[i])==tolower(palabraAux2[i])){
		i++;
		if(palabraAux1[i]=='\0') return palabraAux1;
		if(palabraAux2[i]=='\0') return palabraAux1;
	}
	if(palabraAux1[i]<palabraAux2[i]) return palabraAux1;
	else return palabraAux2;
}

void crearArchivo(char* contenido,char* rutaArchivoACrear){
	FILE* fd;
	fd = fopen(rutaArchivoACrear,"w");
	if (fd==NULL) {
		printf("Error al abrir el archivo.");
	}
	fputs(contenido,fd);
	fclose(fd);
}

char* obtenerSoloNombreDelArchivo(char* ruta){
		char** aux = string_split(ruta,"/");
		int tamanioAux=0;
		while(aux[tamanioAux]!='\0'){
			tamanioAux++;
		}
		tamanioAux--;
		char* nombreArchivoTemporal = aux[tamanioAux];

		return nombreArchivoTemporal;
}
int tamanioArchivoCerrado(const char* rutaArchivo){
	FILE* fd;
	fd = fopen(rutaArchivo,"r");
	fseek(fd, 0L, SEEK_END);
	int tamanio = ftell(fd);
	fclose(fd);

	return tamanio;
}
