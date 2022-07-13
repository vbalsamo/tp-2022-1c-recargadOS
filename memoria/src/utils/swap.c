#include <utils/swap.h>
char * obtnerPathSwap(uint32_t id) {
    char * nombreArchivo = string_new();
    string_append_with_format(&nombreArchivo, "%s/%d.swap", PATH_SWAP ,id);
    return nombreArchivo;
}

void crearArchivoSwap(uint32_t id, uint32_t tamanioProceso){
    char * nombreArchivo =obtnerPathSwap(id);
    int tamanioSwap =  marcosProceso(tamanioProceso)*TAM_PAGINA;
    char * contenidoSwap = string_repeat('0', tamanioSwap);
    FILE * archivo = fopen(nombreArchivo, "w+");
    fwrite(contenidoSwap, sizeof(char) , tamanioSwap, archivo);
    free(nombreArchivo);
    free(contenidoSwap);
    fclose(archivo);
}

void eliminarArchivoSwap(uint32_t id){
    char * nombreArchivo = obtnerPathSwap(id);
    remove(nombreArchivo);
    free(nombreArchivo);
}

void escribirMarcoSwap(void * contenidoMarco, uint32_t numeroPagina, uint32_t id) {
    //mutex_lock
    sleep(RETARDO_SWAP);
    char * nombreArchivo = obtnerPathSwap(id);
    
    int desplazamiento = numeroPagina * TAM_PAGINA;
    FILE * archivo = fopen(nombreArchivo, "r+");
    fseek(archivo, desplazamiento, SEEK_SET);
    fwrite(contenidoMarco, 1, TAM_PAGINA, archivo);
    free(nombreArchivo);
    fclose(archivo);
    //mutex
}

void * leerPaginaSwap(uint32_t numeroPagina, uint32_t id) {
    sleep(RETARDO_SWAP);
    char * nombreArchivo =obtnerPathSwap(id);
    
    int desplazamiento = numeroPagina * TAM_PAGINA;
    FILE * archivo = fopen(nombreArchivo, "r+");
    fseek(archivo, desplazamiento, SEEK_SET);
    void *contenidoPagina = malloc(TAM_PAGINA);
    fread(contenidoPagina, sizeof(char) , TAM_PAGINA, archivo);
    free(nombreArchivo);
    fclose(archivo);
    return contenidoPagina;
}