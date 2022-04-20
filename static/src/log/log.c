#include <commons/log.h>

t_log* iniciar_logger(char* nombre_log, char* nombre_proceso)
{
    t_log* nuevo_logger;
    if((nuevo_logger = log_create(nombre_log, nombre_proceso, 1, LOG_LEVEL_INFO)) == NULL){
        printf("No se pudo crear el log\n");
        exit(1);
    }
    return nuevo_logger;
}