#include <main.h>
void iniciarHilos(){
    log_info(logger, "iniciando hilos..");
    socket_dispatch = malloc(sizeof(int));
    *socket_dispatch = iniciar_servidor(IP, PUERTO_ESCUCHA_DISPATCH);
    socket_interrupt = malloc(sizeof(int));
    *socket_interrupt = iniciar_servidor(IP, PUERTO_ESCUCHA_INTERRUPT); 
    pthread_t thread_dispatch, thread_interrupt;
    servidor_dispatch = obtenerServidor(socket_dispatch, deserializarDispatch, "dispatch");
    servidor_interrupt = obtenerServidor(socket_interrupt, deserializarInterrupt, "interrupt");
    
    pthread_create(&thread_dispatch, NULL, (void*)servidor, (void*)servidor_dispatch);
    pthread_create(&thread_interrupt, NULL, (void*)servidor, (void*)servidor_interrupt);
    
    
    pthread_join(thread_interrupt, NULL);
    pthread_join(thread_dispatch, NULL);
    //servidor(servidor_dispatch);
}
void inicializarVariablesGlobales(char * pathConfig){
    config = config_create(pathConfig);
    IP = config_get_string_value(config, "IP");
    PUERTO_ESCUCHA_DISPATCH = config_get_string_value(config, "PUERTO_ESCUCHA_DISPATCH");
    PUERTO_ESCUCHA_INTERRUPT = config_get_string_value(config, "PUERTO_ESCUCHA_INTERRUPT");
    IP_MEMORIA = config_get_string_value(config, "IP_MEMORIA");
    PUERTO_MEMORIA = config_get_string_value(config, "PUERTO_MEMORIA");
    RETARDO_NOOP = config_get_int_value(config, "RETARDO_NOOP") * 100;
    mutex_interrupcion = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(mutex_interrupcion, NULL); 
    log_info(logger, "Variables de configuracion Leidas");
}
int main(int argc, char* argv[]) {
    logger = log_create("cpu.log", "cpu", true, LOG_LEVEL_INFO);
    validarParametros(argc, argv);
    log_info(logger, "parametros validados");
    char * pathConfig = argv[1];
    inicializarVariablesGlobales(pathConfig);
    //handshake con memoria
    int socket_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA);
    t_traduccion_direcciones* traduccion_direcciones = obtenerTraduccionDeDirecciones(socket_memoria);
    close(socket_memoria);
    log_info(logger, "traduccion de direcciones obtenida de memoria");
    
    iniciarHilos();
    
    //liberar heap
    config_destroy(config);
    free(traduccion_direcciones);
    return 0;
}


