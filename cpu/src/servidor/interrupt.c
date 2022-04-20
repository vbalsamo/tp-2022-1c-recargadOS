#include <servidor/interrupt.h>
void deserializarInterrupt(t_paquete * paquete, uint32_t socket_cliente){
    switch(paquete->codigo_operacion){
		case ALGO:{
            break;
        }
        default:{
            break;
        }
    }
	eliminarPaquete(paquete);
	close(socket_cliente);
}
