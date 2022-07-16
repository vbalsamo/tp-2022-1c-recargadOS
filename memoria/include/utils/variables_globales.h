#ifndef MEMORIA_VARIABLES_ESTRUCTURAS_H_
#define MEMORIA_VARIABLES_ESTRUCTURAS_H_
    #include <commons/config.h>
    #include <commons/log.h>
    #include <stdint.h>
    t_config * config;  
    t_config * ips;
    t_log * logger;
    char * IP;
    char * PUERTO_ESCUCHA;
    uint32_t TAM_MEMORIA;
    uint32_t TAM_PAGINA;
    uint32_t ENTRADAS_POR_TABLA;
    uint32_t RETARDO_MEMORIA;
    char * ALGORITMO_REEMPLAZO;
    uint32_t MARCOS_POR_PROCESO;
    uint32_t RETARDO_SWAP;
    char * PATH_SWAP;
    uint32_t CANTIDAD_FRAMES;
#endif