#include <utils/inicializacion.h>

void validarParametros(int argc, char* argv[]){
    if (argc < 2) {
        fprintf(stderr, "Se esperaba: %s [PATH_CONFIG]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}
void incializarVariablesGlobales(char * pathConfig, char * pathConfigIP) {
    config = config_create(pathConfig);
    ips = config_create(pathConfigIP);
    IP = config_get_string_value(ips, "IP_MEMORIA");
    PUERTO_ESCUCHA = config_get_string_value(ips, "PUERTO_MEMORIA");
    TAM_PAGINA = config_get_int_value(config, "TAM_PAGINA");
    TAM_MEMORIA = config_get_int_value(config, "TAM_MEMORIA");
    ENTRADAS_POR_TABLA = config_get_int_value(config, "ENTRADAS_POR_TABLA");
    RETARDO_MEMORIA = config_get_int_value(config, "RETARDO_MEMORIA")*1000;
    ALGORITMO_REEMPLAZO = config_get_string_value(config, "ALGORITMO_REEMPLAZO");
    MARCOS_POR_PROCESO = config_get_int_value(config, "MARCOS_POR_PROCESO");
    RETARDO_SWAP = config_get_int_value(config, "RETARDO_SWAP")*1000;
    PATH_SWAP = config_get_string_value(config, "PATH_SWAP");
    CANTIDAD_FRAMES = TAM_MEMORIA/TAM_PAGINA;
}

void inicializarMemoria(){


   
}



void eliminarVariablesGlobales(char * pathConfig) {
    config_destroy(config);
    config_destroy(ips);
}
