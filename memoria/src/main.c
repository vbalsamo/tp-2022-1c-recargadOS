#include <main.h>

int main(int argc, char* argv[]) {
    logger = log_create("./memoria.log","memoria", true, LOG_LEVEL_TRACE);
    validarParametros(argc, argv);
    log_info(logger, "parametros validados");
   
    char * pathConfig = argv[1];
    incializarVariablesGlobales(pathConfig);
    log_info(logger, "Variables de configuracion Leidas");

    iniciarEstructurasMemoria();

    int socket = iniciar_servidor(IP, PUERTO_ESCUCHA);
    
    while(1){
        int socket_cliente = esperar_cliente(socket);
        log_info(logger, "se conecto cliente");
        t_paquete * paquete = recibirPaquete(socket_cliente);
        deserializarSegun(paquete, socket_cliente);
    }
    return 0;
}