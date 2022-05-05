#include <utils/inicializacion.h>


void validarParametros(int argc, char* argv[]){
    if (argc < 2) {
        fprintf(stderr, "Se esperaba: %s [PATH_CONFIG]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}

void inicializarEstados(){
    estado_ready = list_create();
    estado_new = list_create();
    estado_exec = list_create();
    estado_exit = list_create();
    estado_susp_ready = list_create();
    estado_blocked = queue_create();
    estado_susp_bloq = queue_create();
}

//TO DO: 
// mostrar_instrucciones_deserializados