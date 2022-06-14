#ifndef MEMORIA_SERVIDOR_H_
#define MEMORIA_SERVIDOR_H_

    #include <stdlib.h>
    #include <stdio.h>
    
    
    #include <commons/string.h>
    #include <sockets/socket.h>
    #include <serializacion/estructuras.h> 
    #include <utils/variables_globales.h>
    
    
    
    void deserializarSegun(t_paquete* paquete, int socket);
#endif
