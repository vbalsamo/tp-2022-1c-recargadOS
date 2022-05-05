#ifndef KERNEL_MAIN_H_
#define KERNEL_MAIN_H_

    #include <stdlib.h>
    #include <stdio.h>
    #include <sockets/socket.h>
    #include <commons/config.h>
    #include <utils/inicializacion.h>
    #include <serializacion/estructuras.h>
    #include <commons/log.h>
    #include <utils/planificador.h>

    void deserializarSegun(t_paquete* paquete, int socket);
#endif
