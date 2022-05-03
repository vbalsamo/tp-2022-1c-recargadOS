#include <utils/ciclo_instruccion.h>

t_instruccion fetch(t_pcb *  pcb){
    t_instruccion instruccion_actual = pcb->instrucciones[pcb->program_counter];
}

bool decode(t_instruccion instruccion){
    return instruccion.identificador == COPY;
}

void fetch_operands(t_instruccion instruccion){
    //solo hace falta buscar operandos cuando la instruccion es COPY
    //instruccion.parametro2 representa la direccion logica del valor a escribir
    //interactuar con memoria para obtener  el dato en esa direccion logica
}

void execute(t_instruccion instruccion, uint32_t tiempoSleep){
    switch (instruccion.identificador){
        case NO_OP:
            sleep(tiempoSleep);
            break;
        case IO:
            // Esta instrucción representa una syscall de I/O bloqueante.
            //Se deberá devolver el PCB actualizado al Kernel junto al
            //tiempo de bloqueo en milisegundos.
            break;
        case READ:
            //Se deberá leer el valor de memoria correspondiente a esa
            //dirección lógica e imprimirlo por pantalla.
            break;
        case COPY:
            //Se deberá escribir en memoria el valor del segundo parámetro
            //en la dirección lógica del primer parámetro.
            break;
        case WRITE:
            // Se deberá escribir en memoria el valor ubicado en la dirección
            //lógica pasada como segundo parámetro, en la dirección lógica
            //pasada como primer parámetro. A efectos de esta etapa, el
            //accionar es similar a la instrucción WRITE ya que el valor a
            //escribir ya se debería haber obtenido en la etapa anterior.
            break; 
        case EXIT:
            //Esta instrucción representa la syscall de finalización del proceso.
            //Se deberá devolver el PCB actualizado al Kernel para su finalización.
            break;
        default:
            break;
    }
}