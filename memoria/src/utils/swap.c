#include <utils/swap.h>


void crearArchivoSwap(uint32_t id, uint32_t tamanioProceso){
    char * nombreArchivo = string_new();
    //tener en cuenta que se podria necesitar redondear el tamano del archivo al tamano de paginas del proceso
    string_append_with_format(&nombreArchivo, "%s/%d.swap", PATH_SWAP ,id);
    int tamanioSwap =  marcosProceso(tamanioProceso)*TAM_PAGINA;
    char * contenidoSwap = string_repeat('0', tamanioSwap);
    FILE * archivo = fopen(nombreArchivo, "w+");
    fwrite(contenidoSwap, sizeof(char) , tamanioSwap, archivo);
    free(nombreArchivo);
    fclose(archivo);
}

void eliminarArchivoSwap(uint32_t id){
    char * nombreArchivo = string_new();
    string_append_with_format(&nombreArchivo, "%s/%d.swap", PATH_SWAP ,id);
    remove(nombreArchivo);
    free(nombreArchivo);
}

void suspenderProceso(t_pcb * pcb){
    //desplazamiento = tam pag * num pag
    //fwrite(tabladepaginas)
}

void desuspenderProceso(t_pcb * pcb){
    
}


//suspension -> agarras el pcb con las instrucciones -> lo copias y haces un write en swap de esas instrucciones