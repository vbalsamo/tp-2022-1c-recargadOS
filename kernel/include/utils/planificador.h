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
    pthread_mutex_t mutex_estado_exit;
    pthread_mutex_t mutex_estado_blocked;
    pthread_mutex_t mutex_pcb_ejecutando;
    pthread_mutex_t mutex_estado_susp_ready;

    sem_t sem_multiprogramacion;
    sem_t sem_hay_pcb_en_new;
    sem_t sem_ready;
    sem_t sem_fin_proceso;
    sem_t sem_block;
    sem_t sem_susp_ready;
    sem_t sem_hay_pcb_esperando_ready;
   
    uint32_t id_pcb_ejecutando;
	t_queue * estado_new;
	t_list * estado_ready;
	t_list * estado_exec;
	t_list * estado_exit;
	t_queue * estado_susp_ready;
	t_queue * estado_blocked;

    t_pcb * iniciarPcb(t_proceso * proceso);
    void inicializarPlanificacion();
    void addEstadoExit(t_pcb * pcb);
    void addEstadoReady(t_pcb * pcb);
    void addEstadoExec(t_pcb * pcb);
    void addEstadoBlocked(t_pcbBloqueado * io);
    void addEstadoSuspBlocked(t_IO * ultimoIO);
    void addEstadoSuspReady(t_pcb * pcb);
    void comunicacionCPU(t_pcb * pcb);
    void comunicacionMemoriaCreacionEstructuras(t_pcb * pcb);
    void comunicacionMemoriaFinalizar(t_pcb *);
    void comunicacionMemoriaDesuspender(t_pcb * pcb);
    void comunicacionMemoriaSuspender(t_pcb * pcb);
    void sumarEspera(void * pcbBloqueado);
    void sumarEsperaSegundos(void * pcbBloqueado);
    t_pcb * planificacionFIFO();
    t_pcb * planificacionSRT();
    void Aready();
    void readyAexec();
    void suspReadyAready();
    void hacerIO (uint32_t tiempoBloqueo);
    void sumarEspera(void * pcbBloqueado);
    void hilo_block();
    void ingresarANew(t_pcb * pcb);
    t_pcb * algoritmoPlanificacion();
    t_pcb * obtenerSiguienteAready();
    void interrumpirPCB();
    void ejecutarPCB(t_pcb * pcb);
    void comunicacionMemoriaDesuspender(t_pcb * pcb);
    void comunicacionMemoriaSuspender(t_pcb * pcb);
        
#endif
