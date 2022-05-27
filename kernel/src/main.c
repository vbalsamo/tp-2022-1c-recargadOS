#include <main.h>

t_proceso * deserializarSegunProceso(t_paquete * paquete, int socket){
    t_proceso * proceso = deserializarProceso(paquete->buffer->stream);
           
            eliminarPaquete(paquete);
            log_info(logger, "se liberó el paquete");
	        close(socket);
            return proceso;
}
/*
void deserializarSegun(t_paquete* paquete, int socket){

	switch(paquete->codigo_operacion){
		case PROCESO:{

            deserializarSegunProceso(paquete);
            //SACAR ESTO DE ACA, solo para conectar al los modulos
        
            
           
			break;
        }
		case ALGO:
			break;
    }
	
}
*/

void manejarProceso(void * socket_cliente){
    int * socket_consola = (int*) socket_cliente;
    t_paquete * paquete = recibirPaquete(*socket_consola);
    t_proceso * nuevo_proceso;
    nuevo_proceso = deserializarSegunProceso(paquete, *socket_consola);

    // for(int i=0; i<nuevo_proceso->sizeInstrucciones; i++){
    //     log_info(logger, "instruccion_id:%d, instruccion_string:%s, parametro1:%d, parametro2:%d",(nuevo_proceso->instrucciones + i)->identificador, instruccion_idAString((nuevo_proceso->instrucciones + i)->identificador), (nuevo_proceso->instrucciones + i)->parametro1, (nuevo_proceso->instrucciones + i)->parametro2);
    // }
    //t_pcb * pcb = 
    iniciarPcb(nuevo_proceso);

    //estructura {socket, pcb, semaforoProcesoN} semaforo distinto para cada conexión
    //wait(planificar)
    //CUIDADO
    
    // uint32_t socket_cpu = crear_conexion(IP_CPU, PUERTO_CPU_DISPATCH);
    // log_info(logger, "conectado a cpu");
    
    // t_paquete * paquete_pcb = armarPaqueteCon(pcb, REQ_PCB_A_EJECUTAR_KERNEL_CPU);
    // enviarPaquete(paquete_pcb, socket_cpu);
    // log_info(logger, "pcb enviado a cpu");
    // eliminarPaquete(paquete_pcb);
    
    // paquete_pcb = recibirPaquete(socket_cpu);
    // log_info(logger, "pcb recibido de cpu");

    // free(pcb);
    // free(nuevo_proceso);
    
    free(socket_consola);
}


int main(int argc, char* argv[]) {

    validarParametros(argc, argv);
    inicializarVariablesGlobales();
    pthread_mutex_init(&mutex_estado_ready, (void *)NULL);
    pthread_mutex_init(&mutex_estado_exit, (void *)NULL);
    pthread_mutex_init(&mutex_estado_new, (void *)NULL);
    sem_init(&sem_multiprogramacion, 0, GRADO_MULTIPROGRAMACION);
    sem_init(&sem_new, 0, 0);
    sem_init(&sem_ready, 0, 0);
    
    t_dictionary * proceso_sockets = dictionary_create();

    //int socket_cliente = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA);
    int * socket = malloc(sizeof(int));
    *socket = iniciar_servidor(IP_KERNEL, PUERTO_ESCUCHA);
    
    // int socketInterrupt = crear_conexion(IP_CPU, PUERTO_CPU_INTERRUPT);
    // t_paquete * paquete = armarPaqueteCon(NULL, REQ_INTERRUPCION_KERNEL_CPU);
    // enviarPaquete(paquete, socketInterrupt);
    
    inicializarEstados();
    inicializarPlanificacion();

    
    //pruebita
    
//atender consola
    while(1){
        // int socket_cliente = esperar_cliente(socket_cliente);

        int * socket_consola = malloc(sizeof(int));
        *socket_consola = esperar_cliente(*socket);

        log_info(logger, "se conecto cliente");

        pthread_t t;
        pthread_create(&t, NULL, (void*) manejarProceso, (void*) socket_consola);
        pthread_detach(t);
 
    }
    
    return 0;
}


