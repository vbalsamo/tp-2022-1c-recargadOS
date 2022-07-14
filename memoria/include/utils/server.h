#ifndef MEMORIA_SERVIDOR_H_
#define MEMORIA_SERVIDOR_H_

    #include <stdlib.h>
    #include <stdio.h>
    #include <commons/string.h>
    #include <commons/collections/queue.h>
    #include <sockets/socket.h>
    #include <serializacion/estructuras.h> 
    #include <utils/variables_globales.h>
    #include <utils/memoria.h>
    #include <utils/swap.h>
    #include <semaphore.h>
    #include <pthread.h>
    
    pthread_mutex_t mutex_colaKernel;
    pthread_mutex_t mutex_colaCPU;
    sem_t sem_CPU;
    sem_t sem_kernel;
    typedef struct{
        t_paquete * paquete;
        int * socket;
    }t_operacion;
    t_queue * colaKernel;
    t_queue * colaCPU;

    void hiloKernel();
    void hiloCPU();

    void deserializarPeticionCPU(t_paquete *paquete, int socket);
    void deserializarPeticionKernel(t_paquete *paquete, int socket);
    bool paqueteDeKernel(t_paquete * paquete);
    void iniciarEstructurasServer();
    void atenderPeticion(int* socket_cliente);
    void deserializarSegun(t_paquete* paquete, int socket);
    
    
#endif
