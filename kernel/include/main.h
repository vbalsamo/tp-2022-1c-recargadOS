#ifndef KERNEL_MAIN_H_
#define KERNEL_MAIN_H_

    #include <stdlib.h>
    #include <stdio.h>
    #include <sockets/server.h>
    #include <commons/config.h>
    #include <utils/inicializacion.h>
    #include <serializacion/estructuras.h>
    #include <sockets/server.h>
    void deserializarSegun(t_paquete* paquete, int socket);
#endif
