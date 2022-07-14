#include <main.h>

int main(int argc, char* argv[]) {
    
    logger = log_create("./memoria.log","memoria", true, LOG_LEVEL_TRACE);
    validarParametros(argc, argv);
    log_info(logger, "parametros validados");
   
    char * pathConfig = argv[1];
    incializarVariablesGlobales(pathConfig);
    log_info(logger, "Variables de configuracion Leidas");
    
    iniciarEstructurasServer();
    iniciarEstructurasMemoria();
    iniciarEstructurasSwap();

    int socket = iniciar_servidor(IP, PUERTO_ESCUCHA);


    while(1){
        int* socket_cliente = malloc(sizeof(int));
        *socket_cliente = esperar_cliente(socket);
        atenderPeticion(socket_cliente);

        //se crea el hilo
        // pthread_t t;
        // pthread_attr_t attr;
        // pthread_attr_init(&attr);
        // pthread_attr_setstacksize(&attr, 16384);
        // pthread_create(&t, &attr, (void*) atenderPeticion, (void*) socket_cliente);
        // pthread_detach(t);

    }
    return 0;
}