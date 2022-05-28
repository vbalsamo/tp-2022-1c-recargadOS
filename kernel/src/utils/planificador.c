#include <utils/planificador.h>

int id_nuevo_proceso = 0;

t_pcb*(*algoritmoPlanificacion)(void);
/*
t_proceso * crearProceso(uint32_t tamanioProceso, uint32_t sizeInstrucciones, t_instruccion * instrucciones){
	t_proceso * proceso = malloc(sizeof(t_proceso));
	proceso->tamanioProceso = tamanioProceso;
	proceso->sizeInstrucciones = sizeInstrucciones;
	proceso->instrucciones = instrucciones;
	return proceso;
*/
t_pcb * iniciarPcb(t_proceso * proceso){
    t_pcb * pcb = malloc(sizeof(t_pcb));
    
    //uint32_t id_pcb = id_nuevo_proceso;
    pcb->id = 0;
    id_nuevo_proceso++;
    pcb->tamanioProceso = proceso->tamanioProceso;
    pcb->sizeInstrucciones = proceso->sizeInstrucciones;
    pcb->instrucciones = proceso->instrucciones;
    pcb->PC = 0;
    pcb->tablaDePaginas = 0; //¿iniciar conexion con memomoria para solicitar tabla de paginas?
    pcb->estimacionRafaga = 0;
    
    
    pthread_mutex_lock(&mutex_estado_new);
    queue_push(estado_new, (void*) pcb);
    pthread_mutex_unlock(&mutex_estado_new);
    log_info(logger, "se agrega proceso id:%d a cola new", pcb->id);
    sem_post(&sem_new);
    
    return pcb;
}

void newAready(){
    while(1){
        log_info(logger, "entró a newAready");
        //tener en cuenta la prioridad suspended
        sem_wait(&sem_multiprogramacion);
        log_info(logger, "pasa multiprogramación");
        sem_wait(&sem_new);
        log_info(logger, "pasó sem_new");
        pthread_mutex_lock(&mutex_estado_new);
        t_pcb * pcb = (t_pcb *) queue_pop(estado_new); //tomo y elimino el primero de la queue_pop
        pthread_mutex_unlock(&mutex_estado_new);
        log_info(logger, "se eliminó el proceso %d de la cola de new", pcb->id);
        
        addEstadoReady(pcb);
        log_info(logger, "se agregó el proceso %d a la lista de ready", pcb->id);
        sem_post(&sem_ready);
    }
}

void readyArunning(){
    while(1){
        sem_wait(&sem_ready);
        t_pcb * pcb = algoritmoPlanificacion();
        
        comunicacionCPU(pcb);
        
        
        
        
        

        addEstadoRunning(pcb);
        log_info(logger, "se agrego a la cola de exit");
        
        //despues pasar a planificador de largo plazo
        sem_post(&sem_multiprogramacion);
    }
}
t_pcb * obtenerSiguientePCB(){
    
    
}

void inicializarPlanificacion(){
    if(string_equals_ignore_case(ALGORITMO_PLANIFICACION,"FIFO")){
        algoritmoPlanificacion = planificacionFIFO();
    }
    else if(string_equals_ignore_case(ALGORITMO_PLANIFICACION,"SRT")){
        algoritmoPlanificacion = planificacionSRT();
    }
    else{
        char * error = string_new();
        string_append_with_format(&error,"Algoritmo de planificacion %d no soportado",ALGORITMO_PLANIFICACION);
        perror(error);
        exit(1);
    }
    pthread_t hilo_newAready;
    pthread_create(&hilo_newAready, NULL, (void*) newAready, NULL);
    pthread_detach(hilo_newAready);
    pthread_t hilo_readyAExit;
    pthread_create(&hilo_readyAExit, NULL, (void*) readyExit, NULL);
    pthread_detach(hilo_readyAExit);
}

t_pcb * planificacionFIFO(){
    t_pcb * pcb;
    pthread_mutex_lock(&mutex_estado_ready);
    pcb = (t_pcb *) list_remove(estado_ready, 0);
    pthread_mutex_unlock(&mutex_estado_ready);

    return pcb;
}
t_pcb * planificacionSRT(){
    //magic 
}

void comunicacionCPU(t_pcb * pcb){
    int socketDispatch = crear_conexion(IP_CPU, PUERTO_CPU_DISPATCH);
    
    t_paquete * paquete = armarPaqueteCon(pcb, REQ_PCB_A_EJECUTAR_KERNEL_CPU);
    enviarPaquete(paquete, socketDispatch);
    free(paquete);

    t_paquete * paqueteRespuesta = recibirPaquete(socketDispatch);
    switch (paquete->codigo_operacion){
        case PCB_EJECUTADO_IO_CPU_KERNEL:{
            
            t_IO * io = deserializarIO(paqueteRespuesta->buffer->stream);
            addEstadoBlocked(io);
            //el hilo de bloqueados es el que se bloquea con usleep(io->tiempoBloqueo)
            break;
        }
        case PCB_EJECUTADO_EXIT_CPU_KERNEL:{
            t_pcb * pcb = deserializarPCB(paqueteRespuesta->buffer->stream, 0);
            //hacerle sem_post al semaforo de la consola de este pcb
            break;
        }
        case PCB_EJECUTADO_INTERRUPCION_CPU_KERNEL:{
            t_pcb * pcb = deserializarPCB(paqueteRespuesta->buffer->stream, 0);
            //enviar a donde corresponda
            break; 
        }   
        default:{
            char * error = string_new();
            string_append_with_format(&error,"Respuesta de CPU %s no soportada",codOPtoString(paquete->codigo_operacion));
            perror(error);
            exit(1);
            break;
        }
            
    }
}


void addEstadoReady(t_pcb * pcb){
    pthread_mutex_lock(&mutex_estado_ready);
    list_add(estado_ready, (void *) pcb);
    pthread_mutex_unlock(&mutex_estado_ready); 
}
void addEstadoExec(t_pcb * pcb){
    pthread_mutex_lock(&mutex_estado_exec);
    list_add(estado_exec, (void *) pcb);
    pthread_mutex_unlock(&mutex_estado_exec); 
}
void addEstadoExit(t_pcb * pcb){
    pthread_mutex_lock(&mutex_estado_exit);
    list_add(estado_exit, (void *) pcb);
    pthread_mutex_unlock(&mutex_estado_exit); 
}
void addEstadoBlocked(t_IO * io){
    pthread_mutex_lock(&mutex_estado_blocked);
    queue_push(estado_blocked, (void *) pcb);
    pthread_mutex_unlock(&mutex_estado_blocked); 
}

/*
void * inicializarPcb(t_proceso * proceso){
    t_pcb * nuevo_pcb = malloc(sizeof(t_pcb));
    nuevo_pcb = crearPcb(proceso);
    list_add(estado_ready, nuevo_pcb);
    return nuevo_pcb;
}
*/