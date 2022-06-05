#ifndef CPU_VARIABLES_ESTRUCTURAS_H_
#define CPU_VARIABLES_ESTRUCTURAS_H_
char * IP;
char * PUERTO_ESCUCHA_DISPATCH;
char * PUERTO_ESCUCHA_INTERRUPT;
char * IP_MEMORIA;
char * PUERTO_MEMORIA;
int RETARDO_NOOP;
t_log * logger;
bool hayInterrupcion;
pthread_mutex_t * mutex_interrupcion;
#endif