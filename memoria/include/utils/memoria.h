#ifndef MEMORIA_MEMORIA_H_
#define MEMORIA_MEMORIA_H_

    #include <stdlib.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <pthread.h>
    #include <utils/variables_globales.h>
    #include <utils/swap.h>
    #include <serializacion/estructuras.h>
    #include <commons/string.h>
    #include <commons/collections/list.h>
    #include <commons/collections/dictionary.h>
    #include <commons/bitarray.h>
    #include <math.h>
    #include <string.h>
    #define ID_SIZE 3



    pthread_mutex_t mutex_tablas_segundo_nivel;
    pthread_mutex_t mutex_tablas_primer_nivel;
    pthread_mutex_t mutex_entrada_segundo_nivel;
    pthread_mutex_t mutex_estados_pcb;
    pthread_mutex_t mutex_id_en_swap;
    pthread_mutex_t mutex_memoria;
    pthread_mutex_t mutex_bitarray;

    t_bitarray * bitarray;
    int numeroMarco;
    void * memoria;
    uint32_t PCB_ID;
    t_list * tablasPrimerNivel;
    t_list * tablasSegundoNivel;
    t_dictionary * estadosPCBS;//t_estadoPCB*

    typedef struct {
        uint32_t marcosOcupados;
        uint32_t punteroClock;
        uint32_t indexTablaPaginasPrimerNivel;
    }t_estadoPCB;

    typedef struct{
        uint32_t id;
        uint32_t tablaSegundoNivel;
    }t_entradaPrimerNivel;

    typedef struct{
        uint32_t id;
        uint32_t marco;
        bool presencia;
        bool uso;
        bool modificado;
        uint32_t paginaSwap;
    }t_entradaSegundoNivel;
    char * stringID(uint32_t id);
    char* asignar_bytes(int cant_frames);
    void iniciarEstructurasMemoria(void);
    void * leerMarco(uint32_t numeroMarco);
    void escribirMarco(uint32_t numeroMarco, void * paginaSwap);
    uint32_t * leerDireccionFisica(uint32_t direccionFisica);
    t_estadoPCB * newEstadoPCB(uint32_t indexTablaPaginasPrimerNivel);
    t_entradaSegundoNivel * crearEntradaSegundoNivel(uint32_t id, uint32_t * ID_EN_SWAP);
    t_entradaPrimerNivel * crearEntradaPrimerNivel(int entradasSegundoNivel, uint32_t id, uint32_t * ID_EN_SWAP);
    t_list * crearTablaSegundoNivel(int entradas, uint32_t id, uint32_t * ID_EN_SWAP);
    uint32_t marcosProceso(uint32_t tamanioProceso);
    uint32_t paginaSegundoNivel(uint32_t paginaSwap);
    uint32_t paginaPrimerNivel(uint32_t paginaSwap);
    uint32_t inicializarEstructurasProceso(t_pcb* pcb);
    t_entradaSegundoNivel * reemplazar(t_estadoPCB * estado, t_entradaSegundoNivel* entrada);
    t_list * obtenerEntradasSegundoNivel(uint32_t indexTablaPaginasPrimerNivel);
    t_entradaSegundoNivel * reemplazarClock(t_estadoPCB * estado, t_list * entradasSegundoNivel, t_entradaSegundoNivel* entrada);
    t_entradaSegundoNivel * reemplazarClockM(t_estadoPCB * estado, t_list * entradasSegundoNivel, t_entradaSegundoNivel* entrada);
    uint32_t obtenerMarco(uint32_t indexTablaSegundoNivel, uint32_t entradaPagina, uint32_t id,bool modificado);
    uint32_t obtenerTablaSegundoNivel(uint32_t indexTablaPrimerNivel, uint32_t entradaPagina);
    void eliminarMarcos(int indexTablaPrimerNivel);
    void eliminarEntradaSegundoNivel(void * entrada);
    void eliminarEntradaPrimerNivel(void * entrada);
    void swapearEntradaPrimerNivel(void * entrada);
    void swapearEntradaSegundoNivel(void * entrada);
    void suspenderProceso(t_pcb * pcb);
    void writeEnMemoria(uint32_t direccionFisica, uint32_t dato);
    void retardoMemoria();
    
#endif
