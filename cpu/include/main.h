#ifndef CPU_MAIN_H_
#define CPU_MAIN_H_

    #include <stdlib.h>
    #include <stdio.h>
    #include <sockets/socket.h>
    #include <commons/config.h>
    #include <commons/log.h>
    #include <utils/inicializacion.h>
    #include <utils/handshake_memoria.h>
    #include <serializacion/estructuras.h>
    #include <servidor/abstract_servidor.h>
    #include <servidor/dispatch.h>
    #include <servidor/interrupt.h>
    #include <pthread.h>
    #include <utils/variables_globales.h>
    #include <utils/ciclo_instruccion.h>
    #include <log/log.h>

    int* socket_dispatch;
    int * socket_interrupt;
    t_servidor * servidor_dispatch;
    t_servidor * servidor_interrupt;
    void iniciar_hilos(void);
    void inicializarVariablesGlobales(char * pathConfig,char * pathConfigIP);
#endif
