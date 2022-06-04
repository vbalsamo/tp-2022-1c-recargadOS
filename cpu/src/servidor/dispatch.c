#include <servidor/dispatch.h>
void deserializarDispatch(t_paquete * paquete, uint32_t socket_cliente){
    switch(paquete->codigo_operacion){
		case REQ_PCB_A_EJECUTAR_KERNEL_CPU:{
            t_pcb * pcb = deserializarPCB(paquete->buffer->stream, 0);
            log_info(logger, "recibido pcb id:%d para ejecutar", pcb->id);
            
            t_paquete* resPaquete = cicloInstruccion(pcb);
            
            enviarPaquete(resPaquete, socket_cliente);
            break;
        }
        default:{

        }
    }
	eliminarPaquete(paquete);
	close(socket_cliente);
}

