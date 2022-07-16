#ifndef CPU_VARIABLES_ESTRUCTURAS_H_
#define CPU_VARIABLES_ESTRUCTURAS_H_
#include <commons/config.h>
#include <commons/log.h>
#include <serializacion/estructuras.h>
t_config * config;
t_config * ips;
char * IP;
char * PUERTO_ESCUCHA_DISPATCH;
char * PUERTO_ESCUCHA_INTERRUPT;
char * IP_MEMORIA;
char * PUERTO_MEMORIA;
char *REEMPLAZO_TLB;
int RETARDO_NOOP;
int ENTRADAS_TLB;
t_log * logger;
bool hayInterrupcion;
pthread_mutex_t mutex_interrupcion;
t_traduccion_direcciones* traduccion_direcciones;
#endif