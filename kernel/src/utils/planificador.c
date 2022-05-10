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
    pcb->tablaDePaginas = NULL;
    pcb->estimacionRafaga = 0;
    printf("id: %d", pcb->id);
    list_add(estado_ready, pcb);
    return pcb;
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