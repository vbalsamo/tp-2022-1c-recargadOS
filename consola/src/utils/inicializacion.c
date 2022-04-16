#include <utils/inicializacion.h>

void validarParametros(int argc, char* argv[]){
    if (argc < 3) {
        fprintf(stderr, "Se esperaban: %s [INSTRUCCIONES_PROCESO], [TAMANIO_PROCESO]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}
char* leerInstrucciones(char* path){
    FILE* fileInstrucciones = fopen(path,"r");
    char* stringInstrucciones = string_new();
    char* instruccionActual = malloc(sizeof(char)*BUFFSIZE);
    fgets(instruccionActual,BUFFSIZE,fileInstrucciones);
    while(!feof(fileInstrucciones)){
        string_append(&stringInstrucciones,instruccionActual);
        instruccionActual=fgets(instruccionActual,BUFFSIZE,fileInstrucciones);
        printf("\n%s",instruccionActual);
    }
    free(instruccionActual);
    free(fileInstrucciones);
    return stringInstrucciones;
}

t_list* obtenerInstrucciones(char* stringInstrucciones){
    
    char** instruccionesSeparadas = string_split(stringInstrucciones,"\n");
    int i = 0;
    t_list* instrucciones = list_create();
    
    while(instruccionesSeparadas[i]!=NULL){
        t_instruccion* instruccion = obtenerInstruccion(instruccionesSeparadas[i]);
        list_add(instrucciones, (void *) instruccion);
        i++;
    }
    return instrucciones;
}