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
    int sizeInstruccionesSeparadas = string_array_size(instruccionesSeparadas)-1;
    *sizeInstrucciones = sizeInstruccionesSeparadas;
    t_instruccion* instrucciones = malloc(sizeof(t_instruccion)*(*sizeInstrucciones));
    
    int k = 0;
    for(int i=0; i<sizeInstruccionesSeparadas;i++){
        instrucciones[k] = obtenerInstruccion(instruccionesSeparadas[i]);
        if(instrucciones[k].identificador==NO_OP){
            *sizeInstrucciones = *sizeInstrucciones + instrucciones[i].parametro1 - 1;
            t_instruccion* nuevasInstrucciones = realloc(instrucciones,sizeof(t_instruccion)*(*sizeInstrucciones));
            instrucciones =nuevasInstrucciones;
            for(int j=i+1; j<i+instrucciones[i].parametro1;j++){
                instrucciones[j] = obtenerInstruccion(instruccionesSeparadas[i]);
            }
            k = i + instrucciones[i].parametro1 - 1;
        }
        k++;
    }
    return instrucciones;
} 