#ifndef MEMORIA_MEMORIA_H_
#define MEMORIA_MEMORIA_H_

    #include <stdlib.h>
    #include <stdio.h>
    #include <utils/variables_globales.h>
    #include <commons/string.h>
    #include <commons/collections/list.h>
    
    void * memoria;
    
    typedef struct{
        t_list * tablasPrimerNivel;
        t_list * tablasSegundoNivel;
    }t_tablas;

    t_tablas * tablas;

    typedef struct{
        uint32_t tablaSegundoNivel;
    }t_entradaPrimerNivel;

    typedef struct{
        uint32_t marco;
        bool presencia;
        bool uso;
        bool modificado;
    }t_entradaSegundoNivel;
    
    void iniciarEstructurasMemoria(void);
#endif
