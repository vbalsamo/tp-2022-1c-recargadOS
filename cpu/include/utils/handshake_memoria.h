#ifndef CPU_UTILS_HANDSHAKE_MEMORIA_H_
#define CPU_UTILS_HANDSHAKE_MEMORIA_H_

    #include <stdlib.h>
    #include <stdio.h>
    #include <serializacion/estructuras.h>
    #include <sockets/socket.h>
    
    t_traduccion_direcciones* obtenerTraduccionDeDirecciones(int socket);

#endif