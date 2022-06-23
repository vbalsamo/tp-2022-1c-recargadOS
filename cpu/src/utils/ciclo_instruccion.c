#include <utils/ciclo_instruccion.h>
#include <utils/variables_globales.h>

t_instruccion fetch(t_pcb *  pcb){
    t_instruccion instruccion_actual = pcb->instrucciones[pcb->PC];
    return instruccion_actual;
}

t_paquete * cicloInstruccion(t_pcb * pcb) {
    t_paquete * paquete;
    bool seguirEjecutando = true;
    t_instruccion instruccion;
    uint32_t PC_inicial = pcb->PC;
    log_info(logger, "Inicia ciclo instruccion para pcb id:%d", pcb->id);
    tablaPaginasPrimerNivelPCB = pcb->tablaDePaginas;

    while(seguirEjecutando ){
        instruccion = fetch(pcb);
        seguirEjecutando = execute(instruccion);
        pcb->PC++;

        pthread_mutex_lock(&mutex_interrupcion);
        if (hayInterrupcion){
            hayInterrupcion=false;
            pthread_mutex_unlock(&mutex_interrupcion);
            log_info(logger, "Hay interrupcion, devulve el pcb");
            paquete = armarPaqueteCon(pcb, PCB_EJECUTADO_INTERRUPCION_CPU_KERNEL);
            return paquete;
        }
        else{
            pthread_mutex_unlock(&mutex_interrupcion);
        }   
        
    }

    pcb->lengthUltimaRafaga = pcb->PC - PC_inicial;
    
    switch (instruccion.identificador){
        case IO:{
            t_IO * io = malloc(sizeof(t_IO));
            io->pcb = pcb;
            io->tiempoBloqueo = instruccion.parametro1;
            log_info(logger, "tiempo bloqueo:%d", instruccion.parametro1);
            paquete = armarPaqueteCon(io, PCB_EJECUTADO_IO_CPU_KERNEL);
            free(io);
            log_info(logger, "Ejecuto IO, devuelve el pcb id:%d", pcb->id);
            break;
        }
        case EXIT:{
            paquete = armarPaqueteCon(pcb, PCB_EJECUTADO_EXIT_CPU_KERNEL);
            log_info(logger, "Ejecuto EXIT, devuelve el pcb id:%d", pcb->id);
            break;
        }
        default:{
           log_error(logger, "No ejecuto EXIT o IO, no debe pasar por aca");
           exit(EXIT_FAILURE);
           break;
        }
    }

    log_info(logger, "finaliza ciclo instruccion para pcb id:%d", pcb->id);
    return paquete;
}


bool execute(t_instruccion instruccion){
    switch (instruccion.identificador){
        case NO_OP:
            log_info(logger, "Ejecutado NO_OP");
            sleep(RETARDO_NOOP);
            return true;
        case IO:
            log_info(logger, "Ejecutado IO");
            return false;
        case READ:
            execute_read(instruccion.parametro1);
            log_info(logger, "Ejecutado READ");
            return true;
        case COPY:{
            uint32_t dato = execute_read(instruccion.parametro2);
            
            log_info(logger, "Ejecutado COPY, Lectura dato en memoria");
            execute_write(instruccion.parametro1, dato);
            
            log_info(logger, "Ejecutado COPY, Escritura dato");
            return true;
        }
        case WRITE:
            
            execute_write( instruccion.parametro1, &instruccion.parametro2);
            log_info(logger, "Ejecutado Write");
            return true;
        case EXIT:
            log_info(logger, "Ejecutado Exit");
            return false;
        default:{
            log_error(logger, "IDENTIFICADOR INSTRUCCION NO CONTEMPLADO-> %d", instruccion.identificador);
            exit(1);
            return false; 
        }
    }
}
uint32_t execute_read(uint32_t direccion_logica){

    uint32_t direccionFisica = consultarDireccionFisica(tablaPaginasPrimerNivelPCB, direccion_logica);
    uint32_t dato = memoria_read(direccionFisica);
    log_info(logger, "READ: %d", dato);
    //leer direccion fisica en memoria y loggear dato
    return dato;
}

void execute_write(uint32_t direccion_logica, void * dato){
    uint32_t direccionFisica = consultarDireccionFisica(tablaPaginasPrimerNivelPCB, direccion_logica);
    memoria_write(direccionFisica, dato);
}

uint32_t memoria_read(uint32_t direccion_fisica) {

    uint32_t socket_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA);

    t_paquete * paquete = armarPaqueteCon(&direccion_fisica, REQ_READ_CPU_MEMORIA);
    enviarPaquete(paquete,socket_memoria);
    t_paquete * paqueteRespuesta = recibirPaquete(socket_memoria);
    uint32_t dato = *deserializarUINT32_T(paqueteRespuesta); //REVISAR PUNTEROS
    return dato;
}

void memoria_write(uint32_t direccion_fisica, uint32_t dato) {

    uint32_t socket_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA);
    t_paquete * paquete = armarPaqueteCon(&direccion_fisica, REQ_WRITE_CPU_MEMORIA);
    enviarPaquete(paquete,socket_memoria);
    t_paquete * paqueteRespuesta = recibirPaquete(socket_memoria);
}