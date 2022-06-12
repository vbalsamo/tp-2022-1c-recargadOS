#ifndef CPU_MMU_H_
#define CPU_MMU_H_

    #include <stdlib.h>
    #include <stdio.h>
    #include <stdint.h>
    #include <math.h>
    #include <stdbool.h>
    #include <utils/variables_globales.h>
    #include <commons/collections/list.h>
    t_list * listaTLB;
    uint32_t tiempoAccesoGlobal;
    uint32_t indiceFIFO;
    
    typedef struct{
        uint32_t pagina;
        uint32_t marco;
        uint32_t tiempoAcceso;
    } t_TLB;

    void iniciarEstructurasMMU();

    t_TLB * new_TLB(uint32_t pagina, uint32_t marco, uint32_t tiempoAcceso);
    
    void reemplazarTLB_FIFO(uint32_t pagina, uint32_t marco);

    void reemplazarTLB_LRU(uint32_t pagina, uint32_t marco);

    t_TLB * obtenerEntradaTLB(uint32_t pagina);

    void incrementarIndiceFIFO();

    uint32_t obtenerNumeroPagina(uint32_t direccion_logica);

    uint32_t obtenerEntradaTabla1erNivel(uint32_t numero_pagina);

    uint32_t obtenerEntradaTabla2doNivel(uint32_t numeroPnumero_paginaagina);

    uint32_t obtenerDesplazamiento(uint32_t direccion_logica, uint32_t numero_pagina);
#endif