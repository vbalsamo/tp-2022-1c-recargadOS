#ifndef CPU_SERVIDOR_ABSTRACT_SERVIDOR_H_
#define CPU_SERVIDOR_ABSTRACT_SERVIDOR_H_
    #include <stdlib.h>
	#include <stdint.h>
    #include <serializacion/estructuras.h>
    #include <sockets/socket.h>
    #include <utils/variables_globales.h>
    typedef struct{
        uint32_t * socket;
        char nombre[10];
        void  (*deserializarSegun)(t_paquete*, uint32_t);
    }t_servidor;
    void servidor(void * params);
    t_servidor * obtenerServidor(uint32_t* socket, void(*deserializarSegun)(t_paquete*,uint32_t), char nombre[10]);
#endif