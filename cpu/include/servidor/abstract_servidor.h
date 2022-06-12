#ifndef CPU_SERVIDOR_ABSTRACT_SERVIDOR_H_
#define CPU_SERVIDOR_ABSTRACT_SERVIDOR_H_
    #include <stdlib.h>
	#include <stdint.h>
    #include <serializacion/estructuras.h>
    #include <sockets/socket.h>
    #include <utils/variables_globales.h>
    typedef struct{
        int * socket;
        char nombre[10];
        void  (*deserializarSegun)(t_paquete*, int);
    }t_servidor;
    void servidor(void * params);
    
    t_servidor * obtenerServidor(int* socket, void(*deserializarSegun)(t_paquete*,int), char nombre[10]);
#endif