#ifndef CPU_MMU_H_
#define CPU_MMU_H_

    #include <stdlib.h>
    #include <stdio.h>
    #include <stdint.h>
    #include <math.h>
    #include <stdbool.h>
    #include <utils/variables_globales.h>
    #include <commons/collections/list.h>
    #include <commons/string.h>
    #include <sockets/socket.h>
    t_list * listaTLB;
    uint32_t tiempoAccesoGlobal;
    uint32_t indiceFIFO;
    uint32_t PCB_ACTUAL;

    typedef struct{
        uint32_t pagina;
        uint32_t marco;
        uint32_t tiempoAcceso;
    } t_EntradaTLB;

    void iniciarEstructurasMMU();

    t_EntradaTLB * newEntradaTLB(uint32_t pagina, uint32_t marcoEXIT_ERROR);
    
    void reemplazarTLB_FIFO(uint32_t pagina, uint32_t marco);

    void reemplazarTLB_LRU(uint32_t pagina, uint32_t marco);

    t_EntradaTLB * obtenerEntradaTLB(uint32_t pagina);
    
    void imprimirEntradaTLB(t_EntradaTLB* entrada);

    void imprimirEntradasTLB();

    void agregarTLB(uint32_t pagina, uint32_t marco);
    
    void actualizarTLB(t_EntradaTLB * entrada);
    
    void incrementarIndiceFIFO();

    uint32_t consultarTablaSegundoNivel(uint32_t tablaDePaginasPrimerNivel, uint32_t pagina);

    uint32_t consultarMarco(uint32_t tablaDePaginasSegundoNivel, uint32_t pagina, t_cod_op codOP);
    
    uint32_t consultarDireccionFisica(uint32_t tablaPaginasPrimerNivelPCB, uint32_t direccion_logica, t_cod_op codOP);

    uint32_t obtenerNumeroPagina(uint32_t direccion_logica);

    uint32_t obtenerEntradaTabla1erNivel(uint32_t numero_pagina);

    uint32_t obtenerEntradaTabla2doNivel(uint32_t numeroPnumero_paginaagina);

    uint32_t obtenerDesplazamiento(uint32_t direccion_logica, uint32_t numero_pagina);
    
    uint32_t obtenerDireccionFisica(uint32_t desplazamiento, uint32_t numero_marco);

    void vaciarTLB(uint32_t nuevo_id);
#endif