#ifndef STATIC_LOG_H_
#define STATIC_LOG_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <commons/string.h>
#include <commons/log.h>
#include <commons/temporal.h>

t_log* iniciar_logger(char* log_path, char* nombre_proceso);

char * obtenerPathLog();

#endif