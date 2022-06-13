#include <servidor/interrupt.h>
void deserializarInterrupt(t_paquete * paquete, int socket_cliente){
    switch(paquete->codigo_operacion){
		case REQ_INTERRUPCION_KERNEL_CPU:{
            log_info(logger, "interrupt:recibido interrupcion");
            
            uint32_t * mensaje = deserializarUINT32_T(paquete->buffer->stream);
            free(mensaje);

            pthread_mutex_lock(&mutex_interrupcion);
            hayInterrupcion=true;
            pthread_mutex_unlock(&mutex_interrupcion);
            break; 
        }
        default:{
            log_error(logger, "interrupt: no es por aca.");
            break;
        }
    }
	eliminarPaquete(paquete);
	close(socket_cliente);
}

void servidorInterrupt(void * socket){
    int * socket_servidor = (int * )socket;
    while(1){
        int socket_cliente = esperar_cliente(*socket_servidor);
        log_info(logger, "interrupt: se conecta cliente");
        //instanciar hilo que atienda la solicitud
        t_paquete * paquete = recibirPaquete(socket_cliente);
        deserializarInterrupt(paquete, socket_cliente);
    }
}
