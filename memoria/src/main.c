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
        case REQ_CREAR_PROCESO_KERNEL_MEMORIA:{
            log_info(logger, "se solicita crear proceso");
            uint32_t tablaPaginas1erNivel;

            //posta magic??
            
            t_paquete* paqueteRespuesta = armarPaqueteCon(tablaPaginas1erNivel, RES_CREAR_PROCESO_KERNEL_MEMORIA);
            enviarPaquete(paqueteRespuesta,socket);
            eliminarPaquete(paqueteRespuesta);
            log_info(logger, "se envia tabla Paginas 1er Nivel");
			break;
        }
        case REQ_FIN_PROCESO_KERNEL_MEMORIA:{
            uint32_t * id = deserializarUINT32_T(paquete->buffer->stream);
            log_info(logger, "se solicita borrar memora y swap del proceso: %d", *id);
        }
		default:{
            break;
        }
			
    }
	eliminarPaquete(paquete);
	close(socket);
}

int main(int argc, char* argv[]) {
    logger = log_create("./memoria.log","memoria", true, LOG_LEVEL_TRACE);
    validarParametros(argc, argv);
    log_info(logger, "parametros validados");
   
    char * pathConfig = argv[1];

    t_config * config = config_create(pathConfig);
    char * IP = config_get_string_value(config, "IP");
    char * PUERTO_ESCUCHA = config_get_string_value(config, "PUERTO_ESCUCHA");
    TAM_PAGINA = config_get_int_value(config, "TAM_PAGINA");
    TAM_PAGINA = config_get_int_value(config, "TAM_PAGINA");
    PAGINAS_POR_TABLA = config_get_int_value(config, "ENTRADAS_POR_TABLA");
    log_info(logger, "Variables de configuracion Leidas");


    int socket = iniciar_servidor(IP, PUERTO_ESCUCHA);

    //hilo
    while(1){
        uint32_t socket_cliente = esperar_cliente(socket);
        log_info(logger, "se conecto cliente");
        t_paquete * paquete = recibirPaquete(socket_cliente);
        deserializarSegun(paquete, socket_cliente);
    }
    return 0;
}


