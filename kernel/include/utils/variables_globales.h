#ifndef KERNEL_VARIABLES_ESTRUCTURAS_H_
#define KERNEL_VARIABLES_ESTRUCTURAS_H_
    #include <commons/log.h>
    #include<commons/config.h>
    t_log * logger;
    t_config * config;
    char * IP_KERNEL;
    char * PUERTO_ESCUCHA;
    char * IP_CPU;
    char * PUERTO_CPU_DISPATCH;
    char * PUERTO_CPU_INTERRUPT;
    char * IP_MEMORIA;
    char * PUERTO_MEMORIA;
    int GRADO_MULTIPROGRAMACION;
    char * ALGORITMO_PLANIFICACION;

    typedef struct {
        int * socket;
        int id;
    }t_consola;
    t_list * consolas_conectadas;
    pthread_mutex_t mutex_consolas_conectadas;
#endif