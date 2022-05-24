#include <main.h>


int main(int argc, char* argv[]) {

    validarParametros(argc, argv);
    
    char * stringInstrucciones = leerInstrucciones(argv[1]);
    
    uint32_t tamanioProceso = atoi(argv[2]);
    //kernel obtiene la lista
    //t_list* instrucciones = obtenerInstrucciones(stringInstrucciones);
    uint32_t * sizeInstrucciones=malloc(sizeof(uint32_t));
    t_instruccion* instrucciones = obtenerInstrucciones(stringInstrucciones,sizeInstrucciones);
    logger = log_create("consola.log", "consola", true, LOG_LEVEL_INFO);
    for(int i = 0; i<*sizeInstrucciones; i++){
        log_info(logger, "instruccion_id:%d, instruccion_string:%s, parametro1:%d, parametro2:%d",(instrucciones + i)->identificador, instruccion_idAString((instrucciones + i)->identificador), (instrucciones + i)->parametro1, (instrucciones + i)->parametro2);
    }
    t_config* config = config_create("/home/utnso/tp-2022-1c-recargadOS/consola/config/consola.cfg");
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
    return 0;
}
