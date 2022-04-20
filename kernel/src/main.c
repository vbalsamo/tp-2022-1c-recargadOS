#include <main.h>
void deserializarSegun(t_paquete* paquete, int socket){

	switch(paquete->codigo_operacion){
		case PROCESO:{
            t_proceso * proceso = deserializarProceso(paquete->buffer->stream);
            
            free(proceso);
			break;
        }
			
		case ALGO:
			break;
    }
	eliminarPaquete(paquete);
	close(socket);
}

t_log* iniciar_logger(void)
{
    t_log* nuevo_logger;
    if((nuevo_logger = log_create("kernellog.log", "TP0", 1, LOG_LEVEL_INFO)) == NULL){
        printf("No se pudo crear el log\n");
        exit(1);
    }
    return nuevo_logger;
}

int main(int argc, char* argv[]) {  
    
    validarParametros(argc, argv);
    t_config * config = config_create(argv[1]);
    char * IP_KERNEL = config_get_string_value(config, "IP_KERNEL");
    char * PUERTO_ESCUCHA = config_get_string_value(config, "PUERTO_ESCUCHA");
   
    int socket = iniciar_servidor(IP_KERNEL, PUERTO_ESCUCHA);

    t_log * nuevolog = iniciar_logger();
    log_info(nuevolog, "soy un log");

    while(1){
        int socket_cliente = esperar_cliente(socket);
        t_paquete * paquete = recibirPaquete(socket_cliente);
        deserializarSegun(paquete, socket_cliente);
        break;
    }

    
    return 0;
}


