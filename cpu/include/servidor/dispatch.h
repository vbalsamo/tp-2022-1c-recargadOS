#ifndef CPU_SERVIDOR_DISPATCH_H_
#define CPU_SERVIDOR_DISPATCH_H_

    #include <stdlib.h>
    #include <stdio.h>
    #include <servidor/abstract_servidor.h>
    #include <utils/ciclo_instruccion.h>
    #include <utils/variables_globales.h>
    #include <utils/mmu.h>
    void deserializarDispatch(t_paquete * paquete, int socket_cliente);
#endif