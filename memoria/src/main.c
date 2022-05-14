#include <main.h>

typedef struct {

}config_memoria;

uint32_t PAGINAS_POR_TABLA;
uint32_t TAM_PAGINA;
t_log * logger;

void deserializarSegun(t_paquete* paquete, int socket){

	switch(paquete->codigo_operacion){
		case REQ_TRADUCCION_DIRECCIONES_CPU_MEMORIA:{
            log_info(logger, "se solicita traducciones de direciones");
            t_traduccion_direcciones * traduccionDirecciones = malloc(sizeof(t_traduccion_direcciones));
            

            traduccionDirecciones->tamanio_pagina = TAM_PAGINA;
            traduccionDirecciones->paginas_por_tabla = PAGINAS_POR_TABLA;
            t_paquete* paquete = armarPaqueteCon(traduccionDirecciones, RES_TRADUCCION_DIRECCIONES_MEMORIA_CPU);
            enviarPaquete(paquete,socket);
            log_info(logger, "se envia traducciones de direciones");
			break;
        }
        case REQ_DATO_DIRECCION_LOGICA_CPU_MEMORIA:{
            log_info(logger, "se solicita dato en direccion de memoria");
            t_mensaje * mensaje = malloc(sizeof(t_mensaje));
            mensaje->longitud = 12;
            mensaje->texto = "IMPLEMENTAR";
            t_paquete* paquete = armarPaqueteCon(mensaje, RES_DATO_DIRECCION_LOGICA_MEMORIA_CPU);
            enviarPaquete(paquete,socket);
            log_info(logger, "se envia dato en direccion de memoria");
			break;
        }
        case REQ_ESCRIBIR_DIRECCION_LOGICA_CPU_MEMORIA:{
            log_info(logger, "se solicita escribir dato en direccion de memoria");
            t_mensaje * mensaje = malloc(sizeof(t_mensaje));
            mensaje->longitud = 12;
            mensaje->texto = "IMPLEMENTAR";
            t_paquete* paquete = armarPaqueteCon(mensaje, RES_ESCRIBIR_DIRECCION_LOGICA_MEMORIA_CPU);
            enviarPaquete(paquete,socket);
            log_info(logger, "se escribio dato en direccion de memoria");
			break;
        }
		default:{
            break;
        }
			
    }
	eliminarPaquete(paquete);
	close(socket);
}

int main(int argc, char* argv[]) {
    validarParametros(argc, argv);
    t_config * config = config_create(argv[1]);
    logger = log_create("./memoria.log","memoria", true, LOG_LEVEL_TRACE);
    
    char * IP_MEMORIA = config_get_string_value(config, "IP_MEMORIA");
    char * PUERTO_ESCUCHA = config_get_string_value(config, "PUERTO_ESCUCHA");
    TAM_PAGINA = config_get_int_value(config, "TAM_PAGINA");
    PAGINAS_POR_TABLA = config_get_int_value(config, "PAGINAS_POR_TABLA");
    int socket = iniciar_servidor(IP_MEMORIA, PUERTO_ESCUCHA);
    log_info(logger, "variables configuracion obtenidas");

    
    //hilo
    while(1){
        uint32_t socket_cliente = esperar_cliente(socket);
        log_info(logger, "se conecto cliente");
        t_paquete * paquete = recibirPaquete(socket_cliente);
        deserializarSegun(paquete, socket_cliente);
    }
    return 0;
}


