#ifndef MEMORIA_MEMORIA_H_
#define MEMORIA_MEMORIA_H_

    #include <stdlib.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <utils/variables_globales.h>
    #include <utils/swap.h>
    #include <serializacion/estructuras.h>
    #include <commons/string.h>
    #include <commons/collections/list.h>
    #include <commons/bitarray.h>
    #include <math.h>
    #include <string.h>

    uint32_t ID_EN_SWAP;

    t_bitarray * bitarray;
    int numeroMarco;
    void * memoria;
    uint32_t PCB_ID;
    t_list * tablasPrimerNivel;
    t_list * tablasSegundoNivel;

    typedef struct{
        t_list * tabla;//(t_entradaPrimerNivel)
    }t_tablaPrimerNivel;

    typedef struct{
        uint32_t tablaSegundoNivel;
    }t_entradaPrimerNivel;
    
    typedef struct{
        t_list * tabla;//(t_entradaSegundoNivel)
    }t_tablaSegundoNivel;

    typedef struct{
        uint32_t marco;
        bool presencia;
        bool uso;
        bool modificado;
        uint32_t paginaSwap;
    }t_entradaSegundoNivel;
    char* asignar_bytes(int cant_frames);
    void iniciarEstructurasMemoria(void);
    void * leerMarco(uint32_t numeroMarco);
    t_entradaSegundoNivel * crearEntradaSegundoNivel();
    t_entradaPrimerNivel * crearEntradaPrimerNivel(int entradasSegundoNivel);
    t_list * crearTablaSegundoNivel(int entradas);
    uint32_t marcosProceso(uint32_t tamanioProceso);
    uint32_t inicializarEstructurasProceso(uint32_t tamanioProceso);
    void eliminarMarcos(int indexTablaPrimerNivel);
    void eliminarEntradaSegundoNivel(void * entrada);
    void eliminarEntradaPrimerNivel(void * entrada);
    void swapearEntradaPrimerNivel(void * entrada);
    void swapearEntradaSegundoNivel(void * entrada);
    void suspenderProceso(t_pcb * pcb);
    
#endif
