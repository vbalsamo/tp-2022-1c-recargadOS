#ifndef CPU_SERVIDOR_DISPATCH_H_
#define CPU_SERVIDOR_DISPATCH_H_

    #include <stdlib.h>
    #include <stdio.h>
    #include <servidor/abstract_servidor.h>
    #include <utils/ciclo_instruccion.h>
    #include <utils/variables_globales.h>
    void deserializarDispatch(t_paquete * paquete, uint32_t socket_cliente);
#endif