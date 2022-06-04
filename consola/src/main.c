#include <main.h>


int main(int argc, char* argv[]) {

    validarParametros(argc, argv);
    char * logPath = obtenerPathLog();

    logger = log_create(logPath, "consola", true, LOG_LEVEL_INFO);
    char * stringInstrucciones = leerInstrucciones(argv[1]);
    log_info(logger, "%s",stringInstrucciones);
    uint32_t tamanioProceso = atoi(argv[2]);
    char * pathConfig = argv[3];
    //kernel obtiene la lista
    //t_list* instrucciones = obtenerInstrucciones(stringInstrucciones);
    uint32_t * sizeInstrucciones=malloc(sizeof(uint32_t));
    t_instruccion* instrucciones = obtenerInstrucciones(stringInstrucciones,sizeInstrucciones);
    // for(int i = 0; i<*sizeInstrucciones; i++){
    //     log_info(logger, "instruccion_id:%d, instruccion_string:%s, parametro1:%d, parametro2:%d",(instrucciones + i)->identificador, instruccion_idAString((instrucciones + i)->identificador), (instrucciones + i)->parametro1, (instrucciones + i)->parametro2);
    // }
    t_config* config = config_create(pathConfig);
    char * IP_KERNEL = config_get_string_value(config,"IP_KERNEL");
    char * PUERTO_KERNEL = config_get_string_value(config,"PUERTO_KERNEL");
    log_info(logger, "variables configuracion obtenidas");
    int socket_cliente = crear_conexion(IP_KERNEL, PUERTO_KERNEL);
    log_info(logger, "conectado a kernel");
    t_proceso* proceso = crearProceso(tamanioProceso, *sizeInstrucciones, instrucciones);
    t_paquete * paquete = armarPaqueteCon(proceso,PROCESO);

    enviarPaquete(paquete , socket_cliente);
    log_info(logger, "proceso enviado a kernel");
    eliminarPaquete(paquete);


    t_paquete * paqueteRespuesta = recibirPaquete(socket_cliente);
    uint32_t * mensaje = deserializarUINT32_T(paqueteRespuesta->buffer->stream);
    log_info(logger, "respuesta kernel: %d", *mensaje);

    close(socket_cliente);
    return 0;
}
