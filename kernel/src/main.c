#include <main.h>



t_proceso * deserializarSegunProceso(t_paquete * paquete, int socket){
    t_proceso * proceso = deserializarProceso(paquete->buffer->stream);
    eliminarPaquete(paquete);
    log_info(logger, "se liberó el paquete");
    //close(socket);
    return proceso;
}



void manejarProceso(int * socket_consola){

    t_paquete * paquete = recibirPaquete(*socket_consola);
    t_proceso * nuevo_proceso = deserializarSegunProceso(paquete, *socket_consola);
    
    // for(int i=0; i<nuevo_proceso->sizeInstrucciones; i++){
    //     log_info(logger, "instruccion_id:%d, instruccion_string:%s, parametro1:%d, parametro2:%d",(nuevo_proceso->instrucciones + i)->identificador, instruccion_idAString((nuevo_proceso->instrucciones + i)->identificador), (nuevo_proceso->instrucciones + i)->parametro1, (nuevo_proceso->instrucciones + i)->parametro2);
    // }

    t_pcb * pcb = iniciarPcb(nuevo_proceso);
    
    t_consola * nuevaConsola = malloc(sizeof(t_consola));
    nuevaConsola->id = pcb->id;
    nuevaConsola->socket = socket_consola;

    pthread_mutex_lock(&mutex_consolas_conectadas);
    list_add(consolas_conectadas, (void *) nuevaConsola);
    pthread_mutex_unlock(&mutex_consolas_conectadas);
    
    ingresarANew(pcb);
    
}


int main(int argc, char* argv[]) {

    validarParametros(argc, argv);
    char * pathConfig = argv[1];
    inicializarVariablesGlobales(pathConfig);
    pthread_mutex_init(&mutex_estado_new, (void *)NULL);
    pthread_mutex_init(&mutex_estado_ready, (void *)NULL);
    pthread_mutex_init(&mutex_pcb_ejecutando, (void *)NULL);
    pthread_mutex_init(&mutex_estado_exit, (void *)NULL);
    pthread_mutex_init(&mutex_estado_blocked, (void *)NULL);
    pthread_mutex_init(&mutex_estado_susp_ready, (void*)NULL);
    pthread_mutex_init(&mutex_consolas_conectadas, (void*)NULL);

    consolas_conectadas = list_create();

    sem_init(&sem_multiprogramacion, 0, GRADO_MULTIPROGRAMACION);
    sem_init(&sem_hay_pcb_en_new, 0, 0);
    sem_init(&sem_ready, 0, 0);
    sem_init(&sem_fin_proceso, 0, 0);   
    sem_init(&sem_block, 0, 0);
    sem_init(&sem_susp_ready, 0, 0);
    sem_init(&sem_hay_pcb_esperando_ready, 0, 0);
    
    

    //int socket_cliente = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA);
    int * socket = malloc(sizeof(int));
    *socket = iniciar_servidor(IP, PUERTO_ESCUCHA);
    
    // int socketInterrupt = crear_conexion(IP_CPU, PUERTO_CPU_INTERRUPT);
    // t_paquete * paquete = armarPaqueteCon(NULL, REQ_INTERRUPCION_KERNEL_CPU);
    // enviarPaquete(paquete, socketInterrupt);
    inicializarEstados();
    inicializarPlanificacion();

    //atender consola
    while(1){

        int * socket_consola = malloc(sizeof(int));
        * socket_consola = esperar_cliente(*socket);

        log_info(logger, "se conecto cliente");
        manejarProceso(socket_consola);
    }
    
    return 0;
}


