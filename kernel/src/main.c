#include <main.h>

void deserializarSegun(t_paquete* paquete, int socket){

	switch(paquete->codigo_operacion){
		case PROCESO:{
            t_proceso * proceso = deserializarProceso(paquete->buffer->stream);
            t_log * nuevolog = log_create("kernel.log", "kernel", 1, LOG_LEVEL_INFO);
            log_info(nuevolog, "Bienvenido al servidor del kernel");
            //printf("%s", proceso->instrucciones->identificador);
            
            
            // int i 
            // for(i=0, i < proceso->sizeInstrucciones, i++){
            //     printf("%s", &proceso.instrucciones[i])
            // }
            // char * structToString(proceso);
            
            
            free(proceso);
			break;
        }
			
		case ALGO:
			break;
    }
	eliminarPaquete(paquete);
	close(socket);
}

int main(int argc, char* argv[]) {  
    
    validarParametros(argc, argv);
    t_config * config = config_create(argv[1]);
    char * IP_KERNEL = config_get_string_value(config, "IP_KERNEL");
    char * PUERTO_ESCUCHA = config_get_string_value(config, "PUERTO_ESCUCHA");

    int socket = iniciar_servidor(IP_KERNEL, PUERTO_ESCUCHA);

    

    while(1){
        int socket_cliente = esperar_cliente(socket);
        t_paquete * paquete = recibirPaquete(socket_cliente);
        deserializarSegun(paquete, socket_cliente);
        break;
    }

    
    return 0;
}


