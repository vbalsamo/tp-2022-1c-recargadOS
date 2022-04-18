#ifndef CONSOLA_INICIALIZACION_H_
#define CONSOLA_INICIALIZACION_H_
    #define BUFFSIZE 20
    #include <stdlib.h>
    #include <stdio.h>
    #include <unistd.h>
    #include <stdint.h>
    #include <string.h>
    #include <commons/string.h>
    #include <commons/collections/list.h>
    #include <instruccion.h>
    void validarParametros(int argc, char* argv[]);
    char* leerInstrucciones(char* path);
    //TODO:llevar al kerner
    t_instruccion* obtenerInstrucciones(char* pathInstruccion, uint32_t* sizeInstrucciones);
#endif
