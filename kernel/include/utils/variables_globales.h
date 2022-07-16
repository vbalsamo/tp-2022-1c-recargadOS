#ifndef KERNEL_VARIABLES_ESTRUCTURAS_H_
#define KERNEL_VARIABLES_ESTRUCTURAS_H_
    #include <commons/log.h>
    #include<commons/config.h>
    t_log * logger;
    t_config * config;
    t_config * ips;
    char * IP;
    char * PUERTO_ESCUCHA;
    char * IP_CPU;
    char * PUERTO_CPU_DISPATCH;
    char * PUERTO_CPU_INTERRUPT;
    char * IP_MEMORIA;
    char * PUERTO_MEMORIA;
    int GRADO_MULTIPROGRAMACION;
    char * ALGORITMO_PLANIFICACION;
    int ESTIMACION_INICIAL;
    double ALFA;
    int TIEMPO_MAXIMO_BLOQUEADO;
    

    typedef struct {
        int * socket;
        int id;
    }t_consola;
    
    t_list * consolas_conectadas;
    pthread_mutex_t mutex_consolas_conectadas;
#endif