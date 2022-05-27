#ifndef KERNEL_PLANIFICADOR_H_
#define KERNEL_PLANIFICADOR_H_

    #include <stdlib.h>
    #include <stdio.h>
    #include <commons/string.h>
    #include <commons/collections/list.h>
    #include <instruccion.h>
    #include <serializacion/estructuras.h>
    #include <semaphore.h>
    #include <pthread.h>
    #include <commons/log.h>
    #include <utils/variables_globales.h>
    pthread_mutex_t mutex_estado_ready;
    pthread_mutex_t mutex_estado_exit;
    pthread_mutex_t mutex_estado_new;
    sem_t sem_multiprogramacion;
    sem_t sem_new;
    sem_t sem_ready;
    

    t_pcb * iniciarPcb(t_proceso * proceso);//void * inicializarPcb(t_proceso * proceso);
    void inicializarPlanificacion();
    void addEstadoExit(t_pcb * pcb);    void addEstadoReady(t_pcb * pcb);
    void readyExit();  
    
    

#endif
