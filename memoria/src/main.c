#include <main.h>

typedef struct {

}config_memoria;

uint32_t PAGINAS_POR_TABLA;
uint32_t TAM_PAGINA;
t_log * log_;

void deserializarSegun(t_paquete* paquete, int socket){

	switch(paquete->codigo_operacion){
		case REQ_TRADUCCION_DIRECCIONES:{
            log_info(log_, "se solicita traducciones de direciones");
            t_traduccion_direcciones * traduccionDirecciones = malloc(sizeof(t_traduccion_direcciones));
            

            traduccionDirecciones->tamanio_pagina = TAM_PAGINA;
            traduccionDirecciones->paginas_por_tabla = PAGINAS_POR_TABLA;
            t_paquete* paquete = armarPaqueteCon(traduccionDirecciones, RES_TRADUCCION_DIRECCIONES);
            enviarPaquete(paquete,socket);
            log_info(log_, "se envia traducciones de direciones");
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
    log_ = log_create("./cpu.log","cpu", true, LOG_LEVEL_TRACE);
    char * IP_MEMORIA = config_get_string_value(config, "IP_MEMORIA");
    char * PUERTO_ESCUCHA = config_get_string_value(config, "PUERTO_ESCUCHA");
    TAM_PAGINA = config_get_int_value(config, "TAM_PAGINA");
    PAGINAS_POR_TABLA = config_get_int_value(config, "PAGINAS_POR_TABLA");
    int socket = iniciar_servidor(IP_MEMORIA, PUERTO_ESCUCHA);

    
    //hilo
    while(1){
        uint32_t socket_cliente = esperar_cliente(socket);
        t_paquete * paquete = recibirPaquete(socket_cliente);
        deserializarSegun(paquete, socket_cliente);
        break;
    }
    return 0;
}


