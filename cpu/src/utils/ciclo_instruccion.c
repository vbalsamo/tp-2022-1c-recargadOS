#include <utils/ciclo_instruccion.h>
#include <utils/variables_globales.h>

t_instruccion fetch(t_pcb *  pcb){
    t_instruccion instruccion_actual = pcb->instrucciones[pcb->PC];
    return instruccion_actual;
}
/*  lock(mutex_interrupcion);
    hilo interrupt -> hayInterrupcion=true;
    unlock(mutex_interrupcion);
*/

t_paquete * cicloInstruccion(t_pcb * pcb) {
    t_paquete * paquete;
    bool hayInterrupcion = false;//TODO:variable compartida con dispatch
    bool seguirEjecutando = true;
    t_instruccion instruccion;
    while(seguirEjecutando ){
        instruccion = fetch(pcb);
        seguirEjecutando = execute(instruccion);
        pcb->PC++;

        //lock(mutex_interrupcion);
        if (hayInterrupcion){
            //unlock(mutex_interrupcion);
            paquete = armarPaqueteCon(pcb, PCB_EJECUTADO_INTERRUPCION_CPU_KERNEL);
            return paquete;
        }
        else{
            //unlock(mutex_interrupcion);
        }   
        
    }
    
    switch (instruccion.identificador){
        case IO:{
            t_IO * io = malloc(sizeof(t_IO));
            io->pcb = pcb;
            io->tiempoBloqueo = instruccion.parametro1;
            paquete = armarPaqueteCon(io, PCB_EJECUTADO_IO_CPU_KERNEL);
        }
        case EXIT:{
            paquete = armarPaqueteCon(pcb, PCB_EJECUTADO_EXIT_CPU_KERNEL);
        }
    }
    
    return paquete;
}


bool execute(t_instruccion instruccion){
    switch (instruccion.identificador){
        case NO_OP:
            for(int i=0; i<instruccion.parametro1; i++){
                sleep(RETARDO_NOOP);
            }
            return true;
        case IO:
            return false;
        case READ:
            execute_read(instruccion.parametro1);
            return true;
        case COPY:{
            //DECODE
            char* dato = execute_read(instruccion.parametro2);//fetch opera
            //solo hace falta buscar operandos cuando la instruccion es COPY
            //instruccion.parametro2 representa la direccion logica del valor a escribir
            //interactuar con memoria para obtener  el dato en esa direccion logica

            execute_write(instruccion.parametro1, dato);
            //Se deberá escribir en memoria el valor del segundo parámetro
            //en la dirección lógica del primer parámetro.
            return true;
        }
        case WRITE:
            // Se deberá escribir en memoria el valor ubicado en la dirección
            //lógica pasada como segundo parámetro, en la dirección lógica
            //pasada como primer parámetro. A efectos de esta etapa, el
            //accionar es similar a la instrucción WRITE ya que el valor a
            //escribir ya se debería haber obtenido en la etapa anterior.
            execute_write(instruccion.parametro1, instruccion.parametro2);
            return true;
        case EXIT:
            return false;
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