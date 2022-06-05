#include <servidor/interrupt.h>
void deserializarInterrupt(t_paquete * paquete, int socket_cliente){
    switch(paquete->codigo_operacion){
		case REQ_INTERRUPCION_KERNEL_CPU:{
            log_info(logger, "recibido interrupcion");
            pthread_mutex_lock(mutex_interrupcion);
            hayInterrupcion=true;
            pthread_mutex_unlock(mutex_interrupcion);
            break;
        }
        default:{
            break;
        }
    }
	eliminarPaquete(paquete);
	close(socket_cliente);
}
