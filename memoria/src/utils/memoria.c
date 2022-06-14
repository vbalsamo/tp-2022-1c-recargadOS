#include <utils/memoria.h>

char* asignar_bytes(int cant_frames) {
    char* buf;
    int bytes;
    if(cant_frames < 8)
        bytes = 1;
    else
    {
        double c = (double) cant_frames;
        bytes = (int) ceil(c/8.0);
    }
    buf = malloc(bytes);
    memset(buf,0,bytes);
    return buf;
}

void iniciarEstructurasMemoria(void) {
    memoria = malloc(TAM_MEMORIA);
    uint32_t CANTIDAD_FRAMES = TAM_MEMORIA/TAM_PAGINA;
    char * bitarrayy = asignar_bytes(CANTIDAD_FRAMES);
    bitarray = bitarray_create(bitarrayy,1);

    tablas = malloc(sizeof(t_tablas));
    tablas->tablasPrimerNivel = list_create();
    tablas->tablasSegundoNivel = list_create();
}
