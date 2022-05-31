#include <utils/inicializacion.h>


void validarParametros(int argc, char* argv[]){
    if (argc < 2) {
        fprintf(stderr, "Se esperaba: %s [PATH_CONFIG]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}

void inicializarEstados(){
    estado_ready = list_create();
    estado_new = queue_create();
    estado_susp_ready = list_create();
    estado_blocked = queue_create();
    estado_susp_blocked = list_create();
    
}

void inicializarVariablesGlobales(){
    config = config_create("/home/utnso/tp-2022-1c-recargadOS/kernel/config/kernel.cfg");
    logger = log_create("kernel.log", "kernel", 1, LOG_LEVEL_INFO);
    IP_KERNEL = config_get_string_value(config, "IP_KERNEL");
    PUERTO_ESCUCHA = config_get_string_value(config, "PUERTO_ESCUCHA");

    IP_CPU = config_get_string_value(config, "IP_CPU");
    PUERTO_CPU_DISPATCH = config_get_string_value(config, "PUERTO_CPU_DISPATCH");
    PUERTO_CPU_INTERRUPT = config_get_string_value(config, "PUERTO_CPU_INTERRUPT");
    IP_MEMORIA = config_get_string_value(config, "IP_MEMORIA"); 
    PUERTO_MEMORIA = config_get_string_value(config,"PUERTO_MEMORIA");
    GRADO_MULTIPROGRAMACION = config_get_int_value(config, "GRADO_MULTIPROGRAMACION");
    ALGORITMO_PLANIFICACION = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
    ALFA = config_get_double_value(config, "ALFA");
    TIEMPO_MAXIMO_BLOQUEADO = config_get_int_value(config, "TIEMPO_MAXIMO_BLOQUEADO");
}

//TO DO: 
// mostrar_instrucciones_deserializados