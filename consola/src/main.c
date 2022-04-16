#include <main.h>


int main(int argc, char* argv[]) {

    validarParametros(argc, argv);
    
    char * stringInstrucciones = leerInstrucciones(argv[1]);
    size_t longitudInstrucciones = strlen(stringInstrucciones);
    u_int32_t tamanioProceso = atoi(argv[2]);
    //kernel obtiene la lista
    //t_list* instrucciones = obtenerInstrucciones(stringInstrucciones);
    
    t_config* config = config_create("/home/juan-dell/git/tp-2022-1c-recargadOS/consola/config/consola.cfg");
    char * IP_KERNEL = config_get_string_value(config,"IP_KERNEL");
    char * PUERTO_KERNEL = config_get_string_value(config,"PUERTO_KERNEL");
 
    int socket_cliente = crear_conexion(IP_KERNEL, PUERTO_KERNEL);
    t_proceso* proceso = crearProceso(tamanioProceso, longitudInstrucciones, stringInstrucciones);
    t_paquete * paquete = armarPaqueteCon(proceso,PROCESO);
    enviarPaquete(paquete , socket_cliente);
    
    return 0;
}
