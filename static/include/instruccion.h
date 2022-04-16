#ifndef STATIC_INSTRUCCION_H_
#define STATIC_INSTRUCCION_H_

    #include <stdlib.h>
    #include <stdio.h>
    #include <commons/string.h>

    typedef enum {
        NO_OP,
        IO,
        READ,
        COPY,
        WRITE,
        EXIT
    }instruccion_id; 


    typedef struct{
        instruccion_id identificador;
        u_int32_t parametro1;
        u_int32_t parametro2;
    }t_instruccion;

    t_instruccion* obtenerInstruccion(char* instruccionTexto);

#endif
