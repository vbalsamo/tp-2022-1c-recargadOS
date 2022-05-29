#ifndef KERNEL_MAIN_H_
#define KERNEL_MAIN_H_

    #include <stdlib.h>
    #include <stdio.h>
    #include <sockets/socket.h>
    #include <commons/config.h>
    #include <commons/collections/dictionary.h>
    #include <commons/collections/list.h>
    #include <utils/inicializacion.h>
    #include <serializacion/estructuras.h>
    #include <commons/log.h>
    #include <utils/planificador.h>
    #include <utils/variables_globales.h>
    #include <utils/inicializacion.h>
    #include <pthread.h>

    void deserializarSegun(t_paquete* paquete, int socket);
    t_proceso * deserializarSegunProceso(t_paquete * paquete, int socket);
#endif
