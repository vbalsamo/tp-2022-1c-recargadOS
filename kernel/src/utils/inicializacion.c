#include <utils/inicializacion.h>

void validarParametros(int argc, char* argv[]){
    if (argc < 2) {
        fprintf(stderr, "Se esperaba: %s [PATH_CONFIG]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}
