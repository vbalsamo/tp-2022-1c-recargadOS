#include <main.h>

t_proceso * nuevo_proceso;

void deserializarSegun(t_paquete* paquete, int socket){

	switch(paquete->codigo_operacion){
		case PROCESO:{
            t_proceso * proceso = deserializarProceso(paquete->buffer->stream);
            nuevo_proceso = proceso;
            //free(proceso);
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
    t_config * config = config_create(argv[1]);
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
        t_paquete * paquete = recibirPaquete(socket_cliente);
        deserializarSegun(paquete, socket_cliente);
        iniciarPcb(nuevo_proceso);
        free(nuevo_proceso);
    }
    
    return 0;
}


