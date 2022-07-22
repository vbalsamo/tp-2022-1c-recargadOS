#include <utils/swap.h>

void iniciarEstructurasSwap() {
    pthread_mutex_init(&mutex_swap, (void *)NULL);
}

char * obtnerPathSwap(uint32_t id) {
    char * nombreArchivo = string_new();
    string_append_with_format(&nombreArchivo, "%s/%d.swap", PATH_SWAP ,id);
    return nombreArchivo;
}

void crearArchivoSwap(uint32_t id, uint32_t tamanioProceso){
    char * nombreArchivo =obtnerPathSwap(id);
    int tamanioSwap =  marcosProceso(tamanioProceso)*TAM_PAGINA;
    char * contenidoSwap = string_repeat('0', tamanioSwap);
    //log_info(logger, "Esperando retardo swap de %d", RETARDO_SWAP/1000);
    //usleep(RETARDO_SWAP);
    FILE * archivo = fopen(nombreArchivo, "w+");
    fwrite(contenidoSwap, sizeof(char) , tamanioSwap, archivo);
    free(nombreArchivo);
    free(contenidoSwap);
    fclose(archivo);
}

void eliminarArchivoSwap(uint32_t id){
    char * nombreArchivo = obtnerPathSwap(id);
    //log_info(logger, "Esperando retardo swap de %d", RETARDO_SWAP/1000);
    //usleep(RETARDO_SWAP);
    remove(nombreArchivo);
    free(nombreArchivo);
}

void escribirMarcoSwap(void * contenidoMarco, uint32_t numeroPagina, uint32_t id) {
    pthread_mutex_lock(&mutex_swap);
    log_info(logger, "Esperando retardo swap de %d milisegundos", RETARDO_SWAP/1000);
    usleep(RETARDO_SWAP);
    char * nombreArchivo = obtnerPathSwap(id);
    
    int desplazamiento = numeroPagina * TAM_PAGINA;
    FILE * archivo = fopen(nombreArchivo, "r+");
    fseek(archivo, desplazamiento, SEEK_SET);
    fwrite(contenidoMarco, 1, TAM_PAGINA, archivo);
    free(nombreArchivo);
    fclose(archivo);
    pthread_mutex_unlock(&mutex_swap);
}

void * leerPaginaSwap(uint32_t numeroPagina, uint32_t id) {
    pthread_mutex_lock(&mutex_swap);
    log_info(logger, "Esperando retardo swap de %d milisegundos", RETARDO_SWAP/1000);
    usleep(RETARDO_SWAP);
    char * nombreArchivo = obtnerPathSwap(id);
    int desplazamiento = numeroPagina * TAM_PAGINA;
    FILE * archivo = fopen(nombreArchivo, "r+");
    fseek(archivo, desplazamiento, SEEK_SET);
    void *contenidoPagina = malloc(TAM_PAGINA);
    fread(contenidoPagina, 1 , TAM_PAGINA, archivo);
    free(nombreArchivo);
    fclose(archivo);
    pthread_mutex_unlock(&mutex_swap);
    return contenidoPagina;
}