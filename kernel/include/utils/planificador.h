#ifndef KERNEL_PLANIFICADOR_H_
#define KERNEL_PLANIFICADOR_H_

    #include <stdlib.h>
    #include <stdio.h>
    #include <semaphore.h>
    #include <pthread.h>
    #include <commons/string.h>
    #include <commons/collections/list.h>
    #include <commons/log.h>
    #include <instruccion.h>
    #include <serializacion/estructuras.h>
    #include <sockets/socket.h>  
    #include <utils/variables_globales.h>
    pthread_mutex_t mutex_estado_new;
    pthread_mutex_t mutex_estado_ready;
    pthread_mutex_t mutex_estado_exec;
    pthread_mutex_t mutex_estado_exit;
    pthread_mutex_t mutex_estado_blocked;
    sem_t sem_multiprogramacion;
    sem_t sem_new;
    sem_t sem_ready;
    sem_t sem_fin_proceso;
    
    

    t_pcb * iniciarPcb(t_proceso * proceso);//void * inicializarPcb(t_proceso * proceso);
    void inicializarPlanificacion();
    void addEstadoExit(t_pcb * pcb);
    void addEstadoReady(t_pcb * pcb);
    void addEstadoExec(t_pcb * pcb);
    void addEstadoBlocked(t_IO * io);
    void comunicacionCPU(t_pcb * pcb);
    t_pcb * planificacionFIFO();
    t_pcb * planificacionSRT();
    void newAready();
    void readyAexec();
    void ingresarANew(t_pcb * pcb);
    t_pcb * algoritmoPlanificacion();

#endif
