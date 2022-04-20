#include <servidor/dispatch.h>
void deserializarDispatch(t_paquete * paquete, uint32_t socket_cliente){
    switch(paquete->codigo_operacion){
		case ALGO:{
            
            break;
        }
        default:{

        }
    }
	eliminarPaquete(paquete);
	close(socket_cliente);
}

