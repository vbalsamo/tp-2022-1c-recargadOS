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
    log_info(logger, "Inicia ciclo instruccion");
    while(seguirEjecutando ){
        instruccion = fetch(pcb);
        seguirEjecutando = execute(instruccion);
        pcb->PC++;

        pthread_mutex_lock(mutex_interrupcion);
        if (hayInterrupcion){
            hayInterrupcion=false;
            pthread_mutex_unlock(mutex_interrupcion);
            log_info(logger, "Hay interrupcion, devulve el pcb");
            paquete = armarPaqueteCon(pcb, PCB_EJECUTADO_INTERRUPCION_CPU_KERNEL);
            return paquete;
        }
        else{
            pthread_mutex_unlock(mutex_interrupcion);
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
            log_info(logger, "Ejecuto IO, devuelve el pcb");
            break;
        }
        case EXIT:{
            paquete = armarPaqueteCon(pcb, PCB_EJECUTADO_EXIT_CPU_KERNEL);
            log_info(logger, "Ejecuto EXIT, devuelve el pcb");
            break;
        }
        default:{
           log_error(logger, "No ejecuto EXIT o IO, no debe pasar por aca");
           break;
        }
    }

    log_info(logger, "finaliza ciclo instruccion");
    return paquete;
}


bool execute(t_instruccion instruccion){
    switch (instruccion.identificador){
        case NO_OP:
            log_info(logger, "Ejecutado NO_OP");
            usleep(RETARDO_NOOP);
            return true;
        case IO:
            log_info(logger, "Ejecutado IO");
            return false;
        case READ:
            execute_read(instruccion.parametro1);
            log_info(logger, "Ejecutado READ");
            return true;
        case COPY:{
            //DECODE
            char* dato = execute_read(instruccion.parametro2);//fetch opera
            //solo hace falta buscar operandos cuando la instruccion es COPY
            //instruccion.parametro2 representa la direccion logica del valor a escribir
            //interactuar con memoria para obtener  el dato en esa direccion logica
            log_info(logger, "Ejecutado COPY, Lectura dato en memoria");
            execute_write(instruccion.parametro1, dato);
            //Se deberá escribir en memoria el valor del segundo parámetro
            //en la dirección lógica del primer parámetro.
            log_info(logger, "Ejecutado COPY, Escritura dato");
            return true;
        }
        case WRITE:
            // Se deberá escribir en memoria el valor ubicado en la dirección
            //lógica pasada como segundo parámetro, en la dirección lógica
            //pasada como primer parámetro. A efectos de esta etapa, el
            //accionar es similar a la instrucción WRITE ya que el valor a
            //escribir ya se debería haber obtenido en la etapa anterior.
            execute_write(instruccion.parametro1, &instruccion.parametro2);
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
void * execute_read(u_int32_t direccion_logica){
    uint32_t socket_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA);
    t_mensaje * mensaje = malloc(sizeof(t_mensaje));
    mensaje->texto=string_new();
    string_append(&mensaje->texto,"Busqueda operando");
    mensaje->longitud=strlen(mensaje->texto)+1;
    t_paquete * paquete = armarPaqueteCon(mensaje,REQ_DATO_DIRECCION_LOGICA_CPU_MEMORIA);
    enviarPaquete(paquete,socket_memoria);
    eliminarPaquete(paquete);

    //respuesta
    paquete = recibirPaquete(socket_memoria);
    if(paquete->codigo_operacion!=RES_DATO_DIRECCION_LOGICA_MEMORIA_CPU){
        perror("No se recibio el codigo de operacion esperado para traduccion de direcciones");
        exit(EXIT_FAILURE);
    }
    t_mensaje* respuesta = deserializarMensaje(paquete->buffer->stream);
    printf("respuesta memoria: %s", respuesta->texto);
    eliminarPaquete(paquete);
    return NULL;
}

void execute_write(u_int32_t direccion_logica, void * dato){
    uint32_t socket_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA);
    t_mensaje * mensaje = malloc(sizeof(t_mensaje));
    mensaje->texto=string_new();
    string_append(&mensaje->texto,"Escribir dato");
    mensaje->longitud=strlen(mensaje->texto)+1;
    t_paquete * paquete = armarPaqueteCon(mensaje,REQ_ESCRIBIR_DIRECCION_LOGICA_CPU_MEMORIA);
    enviarPaquete(paquete,socket_memoria);
    eliminarPaquete(paquete);

    //respuesta
    paquete = recibirPaquete(socket_memoria);
    if(paquete->codigo_operacion!=RES_ESCRIBIR_DIRECCION_LOGICA_MEMORIA_CPU){
        perror("No se recibio el codigo de operacion esperado para traduccion de direcciones");
        exit(EXIT_FAILURE);
    }
    t_mensaje* respuesta = deserializarMensaje(paquete->buffer->stream);
    printf("respuesta memoria: %s", respuesta->texto);
    eliminarPaquete(paquete);
}