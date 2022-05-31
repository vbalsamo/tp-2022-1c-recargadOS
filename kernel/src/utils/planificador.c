#include <utils/planificador.h>

int id_nuevo_proceso = 0;

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
    
    pcb->id = id_nuevo_proceso;
    id_nuevo_proceso++;
    pcb->tamanioProceso = proceso->tamanioProceso;
    pcb->sizeInstrucciones = proceso->sizeInstrucciones;
    pcb->instrucciones = proceso->instrucciones;
    pcb->lengthUltimaRafaga = 0;
    pcb->PC = 0;
    pcb->tablaDePaginas = 0; //¿iniciar conexion con memomoria para solicitar tabla de paginas?
    pcb->estimacionRafaga = 0;

    return pcb;
}

void ingresarANew(t_pcb * pcb){
    pthread_mutex_lock(&mutex_estado_new);
    queue_push(estado_new, (void*) pcb);
    pthread_mutex_unlock(&mutex_estado_new);
    log_info(logger, "se agrega proceso id:%d a cola new", pcb->id);
    sem_post(&sem_hay_pcb_en_new);
}

void newAready(){
    while(1){
        log_info(logger, "entró a newAready");
       
        //tiene prioridad susp-ready->ready

        
        //tener en cuenta la prioridad suspended
        sem_wait(&sem_hay_pcb_en_new);
        log_info(logger, "pasó sem_hay_pcb_en_new");

        sem_wait(&sem_multiprogramacion);
        log_info(logger, "pasa multiprogramación");
        
        
        pthread_mutex_lock(&mutex_estado_new);
        t_pcb * pcb = (t_pcb *) queue_pop(estado_new); //tomo y elimino el primero de la queue_pop
        pthread_mutex_unlock(&mutex_estado_new);

        log_info(logger, "se eliminó el proceso %d de la cola de new", pcb->id);
        
        addEstadoReady(pcb);
        log_info(logger, "se agregó el proceso %d a la lista de ready", pcb->id);
        sem_post(&sem_ready);
        
        
    }
}

void readyAexec(){
    //hilo de ejecucion
    while(1){
        sem_wait(&sem_ready);
        t_pcb * pcb;
 
        pcb = algoritmoPlanificacion();
        bool filtro(void* pcbAux){
        return ((t_pcb*) pcbAux)->id == pcb->id;
        };
        pthread_mutex_lock(&mutex_estado_ready);
        list_remove_by_condition(estado_ready, filtro);
        pthread_mutex_unlock(&mutex_estado_ready);
        comunicacionCPU(pcb);
        //hacer free al pcb

    }
}
//pcb->estimacionRafaga = alfa*pcb->lengthUltimaRafaga + (1-alfa)*pcb->estimacionRafaga

t_pcb* obtenerSiguienteSRT(){

	t_pcb* pcbPlanificado = NULL;
	t_pcb* pcbAux = NULL;
    int i;
	int indexAPlanificar;
	float shortestJob;

	pthread_mutex_lock(&mutex_estado_ready);
	pcbAux = list_get(estado_ready,0);
	pthread_mutex_unlock(&mutex_estado_ready);

	indexAPlanificar = 0;
	shortestJob = pcbAux->estimacionRafaga;

	//itero por la lista de Ready
	//sem_wait(&contadorReady);
	pthread_mutex_lock(&mutex_estado_ready);

	printf("PCBS EN READY: %d \n", list_size(estado_ready));

    for(i=1;i<list_size(estado_ready);i++){
    	pcbAux = list_get(estado_ready,i);
    	
        
    	if(shortestJob > pcbAux->estimacionRafaga){
    		shortestJob = pcbAux->estimacionRafaga;
    		indexAPlanificar = i;
    	}

    }

    pcbPlanificado = list_get(estado_ready, indexAPlanificar);
    pthread_mutex_unlock(&mutex_estado_ready);

	return pcbPlanificado;
}

void execAexit(t_pcb * pcb){
    bool filtro(void* consola){
        return ((t_consola*) consola)->id == pcb->id;
    };
    t_consola * consolaAnotificar;
    
    comunicacionMemoria(pcb);

    pthread_mutex_lock(&mutex_consolas_conectadas);
    consolaAnotificar = list_remove_by_condition(consolas_conectadas, filtro);
    pthread_mutex_unlock(&mutex_consolas_conectadas);
    
    t_paquete * paqueteAconsola = armarPaqueteCon(&(pcb->id), RES_FIN_PROCESO_KERNEL_CONSOLA);
    enviarPaquete(paqueteAconsola, *consolaAnotificar->socket); 
    eliminarPaquete(paqueteAconsola);

    close(*consolaAnotificar->socket);
    free(consolaAnotificar->socket);
    free(consolaAnotificar);
    sem_post(&sem_multiprogramacion);
}

void hilo_block(){
    while(1){
        sem_wait(&sem_block);
        pthread_mutex_lock(&mutex_estado_blocked);
               t_IO * ultimoIO = queue_pop(estado_blocked);
        pthread_mutex_unlock(&mutex_estado_blocked);        
        
        if(ultimoIO->tiempoBloqueo > TIEMPO_MAXIMO_BLOQUEADO){
            usleep(TIEMPO_MAXIMO_BLOQUEADO);
            // Hablar memoria, lo swapeamos
            //addEstadoSuspBlocked(ultimoIO->pcb);
            sem_post(&sem_multiprogramacion);
            usleep(ultimoIO->tiempoBloqueo - TIEMPO_MAXIMO_BLOQUEADO);
            addEstadoSuspReady(ultimoIO->pcb);
        } else{
            usleep(ultimoIO->tiempoBloqueo);
            addEstadoReady(ultimoIO->pcb);
            sem_post(&sem_ready);
        }
        free(ultimoIO);
      }

}

void comunicacionMemoria(t_pcb * pcb) {
    int socketMemoria = crear_conexion(IP_MEMORIA,PUERTO_MEMORIA);
    t_paquete * paqueteAmemoria = armarPaqueteCon(&(pcb->id), REQ_FIN_PROCESO_KERNEL_MEMORIA);
    enviarPaquete(paqueteAmemoria, socketMemoria);
    //CONSULTAR: Esperar confirmacion de Memoria?
}
t_pcb * algoritmoPlanificacion(){
    if(string_equals_ignore_case(ALGORITMO_PLANIFICACION,"FIFO")){
        log_info(logger, "se planifica fifo");
        t_pcb * pcb = planificacionFIFO();
        log_info(logger, "se obtiene pcb->id:%d",pcb->id);
        return pcb;
    }
    else if(string_equals_ignore_case(ALGORITMO_PLANIFICACION,"SRT")){
        log_info(logger, "se planifica srt");
        t_pcb * pcb = planificacionSRT();
        log_info(logger, "se obtiene pcb->id:%d",pcb->id);
        return pcb;
    }
    else{
        char * error = string_new();
        string_append_with_format(&error,"Algoritmo de planificacion %s no soportado", ALGORITMO_PLANIFICACION);
        perror(error);
        exit(1);
    }

}

void inicializarPlanificacion(){

    pthread_t hilo_newAready;
    pthread_create(&hilo_newAready, NULL, (void*) newAready, NULL);
    pthread_detach(hilo_newAready);

    pthread_t hilo_readyAexec;
    pthread_create(&hilo_readyAexec, NULL, (void*) readyAexec, NULL);
    pthread_detach(hilo_readyAexec);

    pthread_t hiloblock;
    pthread_create(&hiloblock, NULL, (void*) hilo_block, NULL);
    pthread_detach(hiloblock);
    
}

t_pcb * planificacionFIFO(){
    t_pcb * pcb;
    pthread_mutex_lock(&mutex_estado_ready);
    pcb = (t_pcb *) list_remove(estado_ready, 0);
    pthread_mutex_unlock(&mutex_estado_ready);

    return pcb;
}

t_pcb * planificacionSRT(){

    t_pcb* pcb = obtenerSiguienteSRT();
    //REVISAR TODOS LOS CASO DE NEW A READY, de supended-ready a ready, el que este ejecutando en ese momento en cpu

    return pcb;
}

// t_pcb * desalojarYEjecutar(t_pcb * pcb){
//     int socket_interrupt = crear_conexion(IP_CPU, PUERTO_CPU_INTERRUPT);
//     t_paquete * paquete = armarPaqueteCon(NULL, REQ_INTERRUPCION_KERNEL_CPU);
//     enviarPaquete(paquete, socket_interrupt);

//     bool filtro(void* pcbFiltro){
//         return ((t_pcb*) pcbFiltro)->id == pcb->id;
//     };
    
//     pthread_mutex_lock(&mutex_estado_ready);
//     t_pcb * pcb = list_remove_by_condition(lista_ready, filtro);
//     pthread_mutex_unlock(&mutex_estado_ready);
//     return pcb;
// }

void comunicacionCPU(t_pcb * pcb){
    int socketDispatch = crear_conexion(IP_CPU, PUERTO_CPU_DISPATCH);
    t_paquete * paquete = armarPaqueteCon(pcb, REQ_PCB_A_EJECUTAR_KERNEL_CPU);
    enviarPaquete(paquete, socketDispatch);
    eliminarPaquete(paquete);
    free(pcb);
    
    t_paquete * paqueteRespuesta = recibirPaquete(socketDispatch);
    switch (paqueteRespuesta->codigo_operacion){
        case PCB_EJECUTADO_IO_CPU_KERNEL:{
            
            t_IO * io = deserializarIO(paqueteRespuesta->buffer->stream);
            //io->pcb->estimacionRafaga = ALFA*(io->pcb->lengthUltimaRafaga) + (1-ALFA)*(io->pcb->estimacionRafaga);
            addEstadoBlocked(io);
            sem_post(&sem_block);
            
            //el hilo de bloqueados es el que se bloquea con usleep(io->tiempoBloqueo)
            break;
        }
        case PCB_EJECUTADO_EXIT_CPU_KERNEL:{
            log_info(logger, "entró un pcb a exit");
            t_pcb * pcbActualizado = deserializarPCB(paqueteRespuesta->buffer->stream, 0);
            execAexit(pcbActualizado);
          
            break;
        }
        case PCB_EJECUTADO_INTERRUPCION_CPU_KERNEL:{
            t_pcb * pcbActualizado = deserializarPCB(paqueteRespuesta->buffer->stream, 0);
            pcbActualizado->estimacionRafaga = ALFA*pcbActualizado->lengthUltimaRafaga + (1-ALFA)*pcbActualizado->estimacionRafaga;
            addEstadoReady(pcbActualizado);
            sem_post(&sem_ready);
            // pthread_mutex_lock(&mutex_pcb_ejecutando);
            // pcbEjecutando = pcbActualizado;
            // pthread_mutex_unlock(&mutex_pcb_ejecutando)            
            //sem_post(&sem_no_suspended_ready);;

            break; 
        }   
        default:{
            char * error = string_new();
            string_append_with_format(&error,"Respuesta de CPU %s no soportada",codOPtoString(paqueteRespuesta->codigo_operacion));
            perror(error);
            exit(1);
            break;
        }
        
    }
    log_info(logger, "CPU ejecutó y devolvió el pcb");  
}


void addEstadoReady(t_pcb * pcb){
    pthread_mutex_lock(&mutex_estado_ready);
    list_add(estado_ready, (void *) pcb);
    pthread_mutex_unlock(&mutex_estado_ready); 
}

void addEstadoBlocked(t_IO * io){
    pthread_mutex_lock(&mutex_estado_blocked);
    queue_push(estado_blocked, (void *) io);
    pthread_mutex_unlock(&mutex_estado_blocked);
}

void addEstadoSuspReady(t_pcb * pcb){
    pthread_mutex_lock(&mutex_estado_susp_ready);
    list_add(estado_susp_ready, (void *) pcb);
    pthread_mutex_unlock(&mutex_estado_susp_ready); 
}/*

// void addEstadoExec(t_pcb * pcb){
//     pthread_mutex_lock(&mutex_estado_exec);
//     list_add(estado_exec, (void *) pcb);
//     pthread_mutex_unlock(&mutex_estado_exec); 
// }
// void addEstadoExit(t_pcb * pcb){
//     pthread_mutex_lock(&mutex_estado_exit);
//     list_add(estado_exit, (void *) pcb);
//     pthread_mutex_unlock(&mutex_estado_exit); 
// }

void * inicializarPcb(t_proceso * proceso){
    t_pcb * nuevo_pcb = malloc(sizeof(t_pcb));
    nuevo_pcb = crearPcb(proceso);
    list_add(estado_ready, nuevo_pcb);
    return nuevo_pcb;
}
*/