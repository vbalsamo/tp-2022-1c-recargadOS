#include <utils/ciclo_instruccion.h>
#include <utils/variables_globales.h>

t_instruccion fetch(t_pcb *  pcb){
    t_instruccion instruccion_actual = pcb->instrucciones[pcb->program_counter];
    return instruccion_actual;
}
/*  t_pcb * pcb;
    while(condicion){
        wait(hayPcb)//otro hilo lo va a activar
        pcb = ciclo_instruccion()
        lock(mutex_interrupcion);
        if hayInterrupcion
            return pcb;
        unlock(mutex_interrupcion);
        post(hayPcb)
    }
*/
/*  lock(mutex_interrupcion);
    hilo interrupt -> hayInterrupcion=true;
    unlock(mutex_interrupcion);
*/
t_pcb * ciclo_instruccion(t_pcb *  pcb, t_instruccion instruccion, uint32_t tiempoSleep){
    switch (instruccion.identificador){
        case NO_OP:
            sleep(tiempoSleep);
            break;
        case IO:
            // hablarlo con los chicos :P
            // Esta instrucción representa una syscall de I/O bloqueante.
            //Se deberá devolver el PCB actualizado al Kernel junto al
            //tiempo de bloqueo en milisegundos.
            return pcb;
            break;
        case READ:
            //Se deberá leer el valor de memoria correspondiente a esa
            //dirección lógica e imprimirlo por pantalla.
            execute_read(instruccion.parametro1);
            break;
        case COPY:{
            //DECODE

            char* dato = execute_read(instruccion.parametro2);//fetch opera
            //solo hace falta buscar operandos cuando la instruccion es COPY
            //instruccion.parametro2 representa la direccion logica del valor a escribir
            //interactuar con memoria para obtener  el dato en esa direccion logica

            execute_write(instruccion.parametro1, dato);
            //Se deberá escribir en memoria el valor del segundo parámetro
            //en la dirección lógica del primer parámetro.
            break;
        }
        case WRITE:
            // Se deberá escribir en memoria el valor ubicado en la dirección
            //lógica pasada como segundo parámetro, en la dirección lógica
            //pasada como primer parámetro. A efectos de esta etapa, el
            //accionar es similar a la instrucción WRITE ya que el valor a
            //escribir ya se debería haber obtenido en la etapa anterior.
            execute_write(instruccion.parametro1, instruccion.parametro2);
            break; 
        case EXIT:
            return pcb;
            break;
        default:
            break;
    }
    pcb->program_counter ++;
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