Master - YAMA

1. Master manda nombre del archivo a YAMA. -> NOMBRE_ARCHIVO 1
4. YAMA manda solicitud de transformación a Master. -> SOL_TRANSFORMACION 2
7. Master manda a YAMA señal de finalización de Transformación. (dando por finalizada la etapa de Transformación) -> FIN_TRANSFORMACION 3
8. YAMA manda a Master solicitud de Reducción Local. Estructura: sol_local (nodo, ip y puerto del worker, archivos temporales de transformación, archivo temporal de reducción local) -> SOL_REDUCCION_LOCAL 4
11. Master manda a YAMA señal de finalización de Reducción Local. (dando por finalizada la etapa de Reducción Local) -> FIN_REDUCCION_LOCAL 5
12. YAMA manda a Master solicitud de Reducción Global, con los datos: Nodo, IP y Puerto del Worker, Archivo Temporal de Reducción Local, Archivo de Reducción Global y Nodo encargado. Estructura: sol_global -> SOL_REDUCCION_GLOBAL 6
15. Master manda a YAMA señal de finalización de Reducción Global. ->FIN_REDUCCION_GLOBAL 7
16. YAMA manda a Master solicitud de Almacenado Final. -> SOL_ALMACENADO_FINAL 8
20. Master manda a Yama señal de finalización de Almacenamiento Final.-> ALMACENADO_FINAL 9

YAMA - FileSystem

2. YAMA manda nombre del archivo a FIleSystem
3. FileSystem manda a YAMA información del archivo. Estructura: datos_archivo









Master - Worker

5. Master manda a cada Worker el bloque a procesar, los bytes ocupados, el nombre del Archivo Temporal y el pgmTransformación. Estructura: infoParaWorker-> 
TAREA_WORKER 16 
6. Worker manda a Master señal de finalización de T