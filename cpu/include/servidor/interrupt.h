#ifndef CPU_SERVIDOR_INTERRUPT_H_
#define CPU_SERVIDOR_INTERRUPT_H_

    #include <stdlib.h>
    #include <stdio.h>
    #include <servidor/abstract_servidor.h>
    #include <pthread.h>
    #include <utils/variables_globales.h>
    #include <serializacion/estructuras.h>
    void deserializarInterrupt(t_paquete * paquete, int socket_cliente);
    void servidorInterrupt(void * socket);
#endif