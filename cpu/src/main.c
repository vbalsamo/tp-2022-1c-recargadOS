#include <main.h>
void iniciarHilos(){
    socket_dispatch = malloc(sizeof(uint32_t));
    *socket_dispatch = iniciar_servidor(IP_CPU, PUERTO_ESCUCHA_DISPATCH);
    socket_interrupt = malloc(sizeof(uint32_t));
    *socket_interrupt = iniciar_servidor(IP_CPU, PUERTO_ESCUCHA_INTERRUPT); 
    pthread_t thread_dispatch, thread_interrupt;
    servidor_dispatch = obtenerServidor(socket_dispatch, deserializarDispatch);
    //servidor_interrupt = obtenerServidor(socket_interrupt, deserializarInterrupt);
    pthread_create(&thread_dispatch, NULL, (void*)servidor, (void*)servidor_dispatch);
    //pthread_create(&thread_interrupt, NULL, (void*)servidor, (void*)servidor_interrupt);
    pthread_join(thread_dispatch, NULL);
    //pthread_join(thread_interrupt, NULL);
}
int main(int argc, char* argv[]) {
    validarParametros(argc, argv);
    t_config * config = config_create(argv[1]);
    //t_log * log = log_create("./cpu.log","CPU",true,LOG_LEVEL_ERROR);
    IP_CPU = config_get_string_value(config, "IP_CPU");
    PUERTO_ESCUCHA_DISPATCH = config_get_string_value(config, "PUERTO_ESCUCHA_DISPATCH");
    PUERTO_ESCUCHA_INTERRUPT = config_get_string_value(config, "PUERTO_ESCUCHA_INTERRUPT");
    IP_MEMORIA = config_get_string_value(config, "IP_MEMORIA");
    PUERTO_MEMORIA = config_get_string_value(config, "PUERTO_MEMORIA");
    RETARDO_NOOP = config_get_int_value(config, "RETARDO_NOOP");
    //handshake con memoria
    uint32_t socket_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA);
    t_traduccion_direcciones* traduccion_direcciones = obtenerTraduccionDeDirecciones(socket_memoria);
    close(socket_memoria);

    iniciar_hilos();
    
    //liberar heap
    free(IP_CPU);
    free(PUERTO_ESCUCHA_DISPATCH);
    free(PUERTO_ESCUCHA_INTERRUPT);
    free(IP_MEMORIA);
    free(PUERTO_MEMORIA);
    free(traduccion_direcciones);
    free(socket_dispatch);
    free(socket_interrupt);
    free(servidor_dispatch);
    free(servidor_interrupt);
    
    return 0;
}


