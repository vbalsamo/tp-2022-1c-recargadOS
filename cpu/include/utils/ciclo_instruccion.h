#ifndef CPU_CICLO_INSTRUCCION_H_
#define CPU_CICLO_INSTRUCCION_H_

    #include <stdlib.h>
    #include <stdio.h>
    #include <instruccion.h>
    #include <serializacion/estructuras.h>
    #include <sockets/socket.h>
    t_instruccion fetch(t_pcb *  pcb);
    bool execute(t_instruccion instruccion);
    void * execute_read(u_int32_t direccion_logica);
    void execute_write(u_int32_t direccion_logica, void * dato);
#endif
