#ifndef CPU_SERVIDOR_INTERRUPT_H_
#define CPU_SERVIDOR_INTERRUPT_H_

    #include <stdlib.h>
    #include <stdio.h>
    #include <servidor/abstract_servidor.h>
    void deserializarInterrupt(t_paquete * paquete, uint32_t socket_cliente);
#endif