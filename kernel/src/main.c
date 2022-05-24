#include <main.h>

t_proceso * nuevo_proceso;

void deserializarSegun(t_paquete* paquete, int socket){

	switch(paquete->codigo_operacion){
		case PROCESO:{
            
            t_proceso * proceso = deserializarProceso(paquete->buffer->stream);

            for(int i=0; i<proceso->sizeInstrucciones; i++){
                log_info(logger, "instruccion_id:%d, instruccion_string:%s, parametro1:%d, parametro2:%d",(proceso->instrucciones + i)->identificador, instruccion_idAString((proceso->instrucciones + i)->identificador), (proceso->instrucciones + i)->parametro1, (proceso->instrucciones + i)->parametro2);
            }
            t_pcb * pcb = iniciarPcb(proceso);
            free(proceso);

            //SACAR ESTO DE ACA, solo para conectar al los modulos
           
            uint32_t socket_cpu = crear_conexion(IP_CPU, PUERTO_CPU_DISPATCH);
            log_info(logger, "conectado a cpu");
            
            t_paquete * paquete = armarPaqueteCon(pcb, REQ_PCB_A_EJECUTAR_KERNEL_CPU);
            enviarPaquete(paquete, socket_cpu);
            log_info(logger, "pcb enviado a cpu");
            eliminarPaquete(paquete);
            
            paquete = recibirPaquete(socket_cpu);
            log_info(logger, "pcb recibido de cpu");
           
			break;
        }
		case ALGO:
			break;
    }
	eliminarPaquete(paquete);
	close(socket);
}



int main(int argc, char* argv[]) {  
    nuevo_proceso = malloc(sizeof(t_proceso));

    validarParametros(argc, argv);
    t_config * config = config_create("/home/utnso/tp-2022-1c-recargadOS/kernel/config/kernel.cfg");
    logger = log_create("kernel.log", "kernel", 1, LOG_LEVEL_INFO);
    IP_KERNEL = config_get_string_value(config, "IP_KERNEL");
    PUERTO_ESCUCHA = config_get_string_value(config, "PUERTO_ESCUCHA");
    IP_CPU = config_get_string_value(config, "IP_CPU");
    PUERTO_CPU_DISPATCH = config_get_string_value(config, "PUERTO_CPU_DISPATCH");
    PUERTO_CPU_INTERRUPT = config_get_string_value(config, "PUERTO_CPU_INTERRUPT");
    IP_MEMORIA = config_get_string_value(config, "IP_MEMORIA"); 
    PUERTO_MEMORIA = config_get_string_value(config,"PUERTO_MEMORIA");
    log_info(logger, "se conecto cliente");
    //int socket_cliente = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA);

    int socket = iniciar_servidor(IP_KERNEL, PUERTO_ESCUCHA);
    
    int socketInterrupt = crear_conexion(IP_CPU, PUERTO_CPU_INTERRUPT);
    t_paquete * paquete = armarPaqueteCon(NULL, REQ_INTERRUPCION_KERNEL_CPU);
    enviarPaquete(paquete, socketInterrupt);

    inicializarEstados();
    
    //pruebita
    


    while(1){
        int socket_cliente = esperar_cliente(socket);
        log_info(logger, "se conecto cliente");
        t_paquete * paquete = recibirPaquete(socket_cliente);
        deserializarSegun(paquete, socket_cliente);

    }
    
    return 0;
}


