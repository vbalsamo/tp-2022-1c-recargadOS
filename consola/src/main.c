#include <main.h>


int main(int argc, char* argv[]) {

    validarParametros(argc, argv);
    
    char * stringInstrucciones = leerInstrucciones(argv[1]);
    
    uint32_t tamanioProceso = atoi(argv[2]);
    //kernel obtiene la lista
    //t_list* instrucciones = obtenerInstrucciones(stringInstrucciones);
    uint32_t * sizeInstrucciones=malloc(sizeof(uint32_t));
    t_instruccion* instrucciones = obtenerInstrucciones(stringInstrucciones,sizeInstrucciones);
    
    t_config* config = config_create("/home/utnso/tp-2022-1c-recargadOS/consola/config/consola.cfg");
    char * IP_KERNEL = config_get_string_value(config,"IP_KERNEL");
    char * PUERTO_KERNEL = config_get_string_value(config,"PUERTO_KERNEL");
 
    int socket_cliente = crear_conexion(IP_KERNEL, PUERTO_KERNEL);
    t_proceso* proceso = crearProceso(tamanioProceso, *sizeInstrucciones, instrucciones);
    t_paquete * paquete = armarPaqueteCon(proceso,PROCESO);
    enviarPaquete(paquete , socket_cliente);
    eliminarPaquete(paquete);

    return 0;
}
