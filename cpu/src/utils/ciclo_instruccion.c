#include <utils/ciclo_instruccion.h>

t_instruccion fetch(t_pcb *  pcb){
    t_instruccion instruccion_actual = pcb->instrucciones[pcb->program_counter];
}

bool decode(t_instruccion instruccion){
    return instruccion.identificador == COPY;
}