#include <main.h>

t_proceso * nuevo_proceso;

void deserializarSegun(t_paquete* paquete, int socket){

	switch(paquete->codigo_operacion){
		case PROCESO:{
            
            t_proceso * proceso = deserializarProceso(paquete->buffer->stream);

            for(int i=0; i<proceso->sizeInstrucciones; i++){
                log_info(log, "instruccion_id:%d, instruccion_string:%s, parametro1:%d, parametro2:%d",(proceso->instrucciones + i)->identificador, instruccion_idAString((proceso->instrucciones + i)->identificador), (proceso->instrucciones + i)->parametro1, (proceso->instrucciones + i)->parametro2);
            }
            iniciarPcb(proceso);

            
            free(proceso);

			break;
        }

        case REQ_TRADUCCION_DIRECCIONES:{
            return;
        }

        case RES_TRADUCCION_DIRECCIONES:{
            return;
        }
			
		case ALGO:
			break;
    }
	eliminarPaquete(paquete);
	close(socket);
}



int main(int argc, char* argv[]) {  
    nuevo_proceso = malloc(sizeof(t_proceso));

    validarParametros(argc, argv);
    t_config * config = config_create("/home/juan-dell/tp-2022-1c-recargadOS/kernel/config/kernel.cfg");
    log = log_create("kernel.log", "kernel", 1, LOG_LEVEL_INFO);
    char * IP_KERNEL = config_get_string_value(config, "IP_KERNEL");
    char * PUERTO_ESCUCHA = config_get_string_value(config, "PUERTO_ESCUCHA");

    char * IP_MEMORIA = config_get_string_value(config, "IP_MEMORIA"); 
    char * PUERTO_MEMORIA = config_get_string_value(config,"PUERTO_MEMORIA");

    int socket_cliente = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA);

    int socket = iniciar_servidor(IP_KERNEL, PUERTO_ESCUCHA); 

    inicializarEstados();

    //pruebita
    


    while(1){
        int socket_cliente = esperar_cliente(socket);
        log_info(log, "se conecto cliente");
        t_paquete * paquete = recibirPaquete(socket_cliente);
        deserializarSegun(paquete, socket_cliente);

    }
    
    return 0;
}


