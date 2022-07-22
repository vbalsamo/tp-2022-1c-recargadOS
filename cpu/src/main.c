#include <main.h>
void iniciarHilos(){
    log_info(logger, "iniciando hilos..");
    socket_dispatch = malloc(sizeof(int));
    *socket_dispatch = iniciar_servidor(IP, PUERTO_ESCUCHA_DISPATCH);
    socket_interrupt = malloc(sizeof(int));
    *socket_interrupt = iniciar_servidor(IP, PUERTO_ESCUCHA_INTERRUPT); 
    pthread_t thread_dispatch, thread_interrupt;
    servidor_dispatch = obtenerServidor(socket_dispatch, deserializarDispatch, "dispatch");
    //servidor_interrupt = obtenerServidor(socket_interrupt, deserializarInterrupt, "interrupt");
    
    pthread_create(&thread_dispatch, NULL, (void*)servidor, (void*)servidor_dispatch);
    pthread_create(&thread_interrupt, NULL, (void*)servidorInterrupt, (void*)socket_interrupt);
    
    
    pthread_detach(thread_interrupt);
    pthread_join(thread_dispatch, NULL);
    //servidor(servidor_dispatch);
}
void inicializarVariablesGlobales(char * pathConfig,char * pathConfigIP){
    config = config_create(pathConfig);
    ips = config_create(pathConfigIP);
    IP = config_get_string_value(ips, "IP_CPU");
    PUERTO_ESCUCHA_DISPATCH = config_get_string_value(ips, "PUERTO_CPU_DISPATCH");
    PUERTO_ESCUCHA_INTERRUPT = config_get_string_value(ips, "PUERTO_CPU_INTERRUPT");
    IP_MEMORIA = config_get_string_value(ips, "IP_MEMORIA");
    PUERTO_MEMORIA = config_get_string_value(ips, "PUERTO_MEMORIA");
    RETARDO_NOOP = config_get_int_value(config, "RETARDO_NOOP") / 1000;
    ENTRADAS_TLB = config_get_int_value(config, "ENTRADAS_TLB");
    REEMPLAZO_TLB = config_get_string_value(config, "REEMPLAZO_TLB");
    hayInterrupcion = false;
    pthread_mutex_init(&mutex_interrupcion, NULL);
    log_info(logger, "Variables de configuracion Leidas");
}
int main(int argc, char* argv[]) {
    char * logPath = obtenerPathLog();
    logger = log_create(logPath, "cpu", true, LOG_LEVEL_INFO);
    validarParametros(argc, argv);
    log_info(logger, "parametros validados");
    char * pathConfig = argv[1];
    char * pathIPS = argv[2];
    inicializarVariablesGlobales(pathConfig, pathIPS);
    //handshake con memoria
    int socket_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA);
    traduccion_direcciones = obtenerTraduccionDeDirecciones(socket_memoria);
    close(socket_memoria);
    log_info(logger, "traduccion de direcciones obtenida de memoria");
    iniciarEstructurasMMU();
    iniciarHilos();
    
    //liberar heap
    config_destroy(config);
    config_destroy(ips);
    free(traduccion_direcciones);
    return 0;
}


