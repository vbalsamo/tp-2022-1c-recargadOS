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
    
    //uint32_t id_pcb = id_nuevo_proceso;
    pcb->id = 0;
    id_nuevo_proceso++;
    pcb->tamanioProceso = proceso->tamanioProceso;
    pcb->sizeInstrucciones = proceso->sizeInstrucciones;
    pcb->instrucciones = proceso->instrucciones;
    pcb->PC = 0;
    pcb->tablaDePaginas = 0; //¿iniciar conexion con memomoria para solicitar tabla de paginas?
    pcb->estimacionRafaga = 0;
    log_info(logger, "se agrega %d", pcb->id);
    pthread_mutex_lock(&mutex_estado_new);
    queue_push(estado_new, (void*) pcb);
    pthread_mutex_unlock(&mutex_estado_new);
    log_info(logger, "paso el mutex");
    sem_post(&sem_new);
    log_info(logger, "signal sem_new");
    return pcb;
}

void newAready(){
    while(1){
        log_info(logger, "entró a newAready");
        sem_wait(&sem_multiprogramacion);
        log_info(logger, "pasa multiprogramación");
        sem_wait(&sem_new);
        log_info(logger, "pasó sem_new");
        pthread_mutex_lock(&mutex_estado_new);
        t_pcb * pcb = (t_pcb *) queue_pop(estado_new); //tomo y elimino el primero de la queue_pop
        pthread_mutex_unlock(&mutex_estado_new);
        log_info(logger, "se elimino el proceso %d de la cola de new", pcb->id);
        
        addEstadoReady(pcb);
        log_info(logger, "se agrego el proceso %d a la lista de ready", pcb->id);
        sem_post(&sem_ready);
    }
}

void addEstadoReady(t_pcb * pcb){
    pthread_mutex_lock(&mutex_estado_ready);
    list_add(estado_ready, (void *) pcb);
    pthread_mutex_unlock(&mutex_estado_ready); 
}
void addEstadoExit(t_pcb * pcb){
    pthread_mutex_lock(&mutex_estado_exit);
    list_add(estado_exit, (void *) pcb);
    pthread_mutex_unlock(&mutex_estado_exit); 
}

void inicializarPlanificacion(){
    pthread_t hilo_newAready;
    pthread_create(&hilo_newAready, NULL, (void*) newAready, NULL);
    pthread_detach(hilo_newAready);
    pthread_t hilo_readyAExit;
    pthread_create(&hilo_readyAExit, NULL, (void*) readyExit, NULL);
    pthread_detach(hilo_readyAExit);
}


//Running a Exit eh!
void readyExit(){
    while(1){
        sem_wait(&sem_ready);
        t_pcb * pcb = (t_pcb *) list_remove(estado_ready, 0); //tomo y elimino el primero de la queue_pop
        //log_info(logger, "se elimino el proceso %d de la cola de ready", pcb->id);
        addEstadoExit(pcb);
        log_info(logger, "se agrego a la cola de exit");
        
        //despues pasar a planificador de largo plazo
        sem_post(&sem_multiprogramacion);
    }
}


/*
void * inicializarPcb(t_proceso * proceso){
    t_pcb * nuevo_pcb = malloc(sizeof(t_pcb));
    nuevo_pcb = crearPcb(proceso);
    list_add(estado_ready, nuevo_pcb);
    return nuevo_pcb;
}
*/
/*
	typedef struct{
		uint32_t id;
		uint32_t tamanioProceso;
		uint32_t sizeInstrucciones;
		t_instruccion* instrucciones;
		uint32_t PC;
		t_list * tablaDePaginas;
		uint32_t estimacionRafaga;
	}t_pcb;
*/

/*
	typedef struct{
		uint32_t id;
		uint32_t tamanioProceso;
		uint32_t sizeInstrucciones;
		t_instruccion* instrucciones;
		uint32_t PC;
		t_list * tablaDePaginas;
		uint32_t estimacionRafaga;
	}t_pcb;
*/