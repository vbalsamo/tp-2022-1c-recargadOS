#include <main.h>
void deserializarSegun(t_paquete* paquete, int socket){

	switch(paquete->codigo_operacion){
		case REQ_TRADUCCION_DIRECCIONES:{
            t_traduccion_direcciones * traduccionDirecciones 
                = deserializarTraduccionDirecciones(paquete->buffer->stream);
            
            free(traduccionDirecciones);
			break;
        }
		default:{
            break;
        }
			
    }
	eliminarPaquete(paquete);
	close(socket);
}

int main(int argc, char* argv[]) {
    validarParametros(argc, argv);
    t_config * config = config_create(argv[1]);
    char * IP_MEMORIA = config_get_string_value(config, "IP_MEMORIA");
    char * PUERTO_ESCUCHA = config_get_string_value(config, "PUERTO_ESCUCHA");
   
    int socket = iniciar_servidor(IP_MEMORIA, PUERTO_ESCUCHA);

    while(1){
        int socket_cliente = esperar_cliente(socket);
        t_paquete * paquete = recibirPaquete(socket_cliente);
        deserializarSegun(paquete, socket_cliente);
        break;
    }
    return 0;
}


