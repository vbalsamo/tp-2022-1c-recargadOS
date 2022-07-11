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
    rewind(fileInstrucciones);
    instruccionActual = fgets(instruccionActual,BUFFSIZE,fileInstrucciones);
    while(!feof(fileInstrucciones)){
        string_append(&stringInstrucciones,instruccionActual);
        instruccionActual = fgets(instruccionActual,BUFFSIZE,fileInstrucciones);
    }
    if(strcmp(instruccionActual,"EXIT")==0){
        string_append(&stringInstrucciones,"EXIT\n");
    }
    
    free(instruccionActual);
    fclose(fileInstrucciones);
    return stringInstrucciones;

}

t_instruccion* obtenerInstrucciones(char* stringInstrucciones, uint32_t* sizeInstrucciones){
    
    char** instruccionesSeparadas = string_split(stringInstrucciones,"\n");
    int sizeInstruccionesSeparadas = string_array_size(instruccionesSeparadas) - 1;
    t_list * lista = list_create();
    for(int i=0; i<sizeInstruccionesSeparadas;i++){
        t_instruccion * instruccion = obtenerInstruccion(instruccionesSeparadas[i]);
        list_add(lista, instruccion);
        if(instruccion->identificador==NO_OP){
            instruccion->parametro2 = 1;
            for(int j = 1; j< instruccion->parametro1; j++){
                t_instruccion * no_op =  obtenerInstruccion(instruccionesSeparadas[i]);
                no_op->parametro2 = j + 1;
                list_add(lista, no_op);
            }
        }
    }

    *sizeInstrucciones = list_size(lista);
    t_instruccion* instrucciones = malloc(sizeof(t_instruccion)*(*sizeInstrucciones));
    for(int i=0; i<*sizeInstrucciones;i++){
        t_instruccion * removed = list_remove(lista,0);
        instrucciones[i] = *removed;
        free(removed);
    }
    string_array_destroy(instruccionesSeparadas);
    list_destroy(lista);
    return instrucciones;
} 