#include <instruccion.h>

t_instruccion * obtenerInstruccion(char* instruccionTexto){
    
    t_instruccion * instruccion = malloc(sizeof(t_instruccion));
    char** instruccionSeparada = string_split(instruccionTexto," ");
    //si es no op generar tantos no op como indica el parametro
    if(string_contains(instruccionSeparada[0],"NO_OP")){
        instruccion->identificador=NO_OP;
        instruccion->parametro1=atoi(instruccionSeparada[1]);
    }
    else if(string_contains(instruccionSeparada[0],"I/O") || string_contains(instruccionSeparada[0], "IO")){
        instruccion->identificador=IO;
        instruccion->parametro1=atoi(instruccionSeparada[1]);
        instruccion->parametro2=0;
    }
    else if(string_contains(instruccionSeparada[0],"READ")){
        instruccion->identificador=READ;
        instruccion->parametro1=atoi(instruccionSeparada[1]);
        instruccion->parametro2=0;
    }
    else if(string_contains(instruccionSeparada[0],"COPY")){
        instruccion->identificador=COPY;
        instruccion->parametro1=atoi(instruccionSeparada[1]);
        instruccion->parametro2=atoi(instruccionSeparada[2]);
    }
    else if(string_contains(instruccionSeparada[0],"WRITE")){
        instruccion->identificador=WRITE;
        instruccion->parametro1=atoi(instruccionSeparada[1]);
        instruccion->parametro2=atoi(instruccionSeparada[2]);
    }
    else if(string_contains(instruccionSeparada[0],"EXIT")){
        instruccion->identificador=EXIT;
        instruccion->parametro1=0;
        instruccion->parametro2=0;
    }
    else{
        fprintf(stderr, "La instruccion: %s no está contemplada\n", instruccionSeparada[0]);
        exit(EXIT_FAILURE);
    }
    string_array_destroy(instruccionSeparada);
    return instruccion;
}

const char* instruccion_idAString(instruccion_id instruccion){
    switch (instruccion) 
    {
        case NO_OP: return "NO_OP";
        case IO: return "IO";
        case READ: return "READ";
        case COPY: return "COPY";
        case WRITE: return "WRITE";
        case EXIT: return "EXIT";

    }
    return "no se pudo traducir la instrucción a string";
}