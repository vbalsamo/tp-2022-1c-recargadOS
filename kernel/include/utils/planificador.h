#ifndef KERNEL_PLANIFICADOR_H_
#define KERNEL_PLANIFICADOR_H_

    #include <stdlib.h>
    #include <stdio.h>
    #include <commons/string.h>
    #include <commons/collections/list.h>
    #include <instruccion.h>
    #include <serializacion/estructuras.h>

    t_pcb * iniciarPcb(t_proceso * proceso);
    //void * inicializarPcb(t_proceso * proceso);
   
    

#endif
