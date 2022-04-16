#include <main.h>
int main(int argc, char* argv[]) {
    validarParametros(argc, argv);
    t_config * config = config_create(argv[1]);
    char * IP_KERNEL = config_get_string_value(config, "IP_KERNEL");
    char * PUERTO_ESCUCHA = config_get_string_value(config, "PUERTO_ESCUCHA");
    int socket = iniciar_servidor(IP_KERNEL, PUERTO_ESCUCHA);

    while(1){
        int socket_cliente = esperar_cliente(socket);
        break;
    }
    return 0;
}
