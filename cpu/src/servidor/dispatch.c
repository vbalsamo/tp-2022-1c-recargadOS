#include <servidor/dispatch.h>
void deserializarDispatch(t_paquete * paquete, uint32_t socket_cliente){
    switch(paquete->codigo_operacion){
		case REQ_PCB_A_EJECUTAR_KERNEL_CPU:{
            t_pcb * pcb;// = deserializar
            t_paquete* paqute = cicloInstruccion(pcb);
            
            enviarPaquete(paquete, socket_cliente);
            break;
        }
        default:{

        }
    }
	eliminarPaquete(paquete);
	close(socket_cliente);
}

