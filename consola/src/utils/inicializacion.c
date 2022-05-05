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

t_instruccion* obtenerInstrucciones(char* stringInstrucciones, uint32_t* sizeInstrucciones){
    
    char** instruccionesSeparadas = string_split(stringInstrucciones,"\n");
    int i = 0;
    *sizeInstrucciones = string_array_size(instruccionesSeparadas) - 1;
    t_instruccion* instrucciones = malloc(sizeof(t_instruccion)*(*sizeInstrucciones));
    
    while(i<*sizeInstrucciones){
        instrucciones[i] = obtenerInstruccion(instruccionesSeparadas[i]);
        //printf("instrucciones: %s %d %d\n", instruccion_idAString(instrucciones[i].identificador), instrucciones[i].parametro1, instrucciones[i].parametro2);
        i++;
    }
    return instrucciones;
} 