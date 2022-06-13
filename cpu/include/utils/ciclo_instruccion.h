#ifndef CPU_CICLO_INSTRUCCION_H_
#define CPU_CICLO_INSTRUCCION_H_

    #include <stdlib.h>
    #include <stdio.h>
    #include <sockets/socket.h>
    #include <pthread.h>
    #include <instruccion.h>
    #include <serializacion/estructuras.h>
    #include <utils/mmu.h>
    
    uint32_t tablaPaginasPrimerNivelPCB;
    t_paquete * cicloInstruccion(t_pcb * pcb);
    t_instruccion fetch(t_pcb *  pcb);
    bool execute(t_instruccion instruccion);
    void * execute_read(uint32_t direccion_logica);
    void execute_write(uint32_t direccion_logica, void * dato);
    void * memoria_read(uint32_t direccion_fisica);
    void memoria_write(uint32_t direccion_fisica, void * dato);
#endif
