#ifndef STATIC_SERIALIZACION_ESTRUCTURAS_H_
#define STATIC_SERIALIZACION_ESTRUCTURAS_H_
	#include <stdlib.h>
	#include <stdint.h>
	#include <string.h>
	#include <unistd.h>
	#include <sys/socket.h>
	#include <netdb.h>
	#include <instruccion.h>
	#include <commons/collections/list.h>
	#include <commons/collections/queue.h>

	typedef enum{
		PROCESO,
		REQ_TRADUCCION_DIRECCIONES,
		RES_TRADUCCION_DIRECCIONES,
		ALGO
	}t_cod_op;

	

	t_list * estado_ready;
	t_list * estado_new;
	t_list * estado_exec;
	t_list * estado_exit;
	t_list * estado_susp_ready;
	t_queue * estado_blocked;
	t_queue * estado_susp_bloq;
	
	
	typedef struct{
        uint32_t size;
        void* stream;
    }t_buffer;

	typedef struct{
        t_cod_op codigo_operacion;
	    t_buffer* buffer;
    }t_paquete;

	typedef struct{
		uint32_t tamanioProceso;
		uint32_t sizeInstrucciones;
		t_instruccion* instrucciones;
	}t_proceso;	
	
	typedef struct{
		uint32_t tamanio_pagina;
		uint32_t paginas_por_tabla;
	}t_traduccion_direcciones;

	typedef struct{
		uint32_t id;
		uint32_t tamanioProceso;
		uint32_t sizeInstrucciones;
		t_instruccion* instrucciones;
		uint32_t PC;
		t_list * tablaDePaginas;
		uint32_t estimacionRafaga;
	}t_pcb;

	void* serializarEstructura(void* estructura,int tamanio,t_cod_op codigoOperacion);
	int tamanioEstructura(void* estructura ,t_cod_op cod_op);
	
	void crearBuffer(t_paquete* paquete);

    t_paquete* crearPaquete(t_cod_op cod_op);
    t_paquete* armarPaqueteCon(void* estructura,t_cod_op cod_op);
    void* serializarPaquete(t_paquete* paquete, uint32_t bytes);
    void enviarPaquete(t_paquete* paquete, int socket_cliente);
    void eliminarPaquete(t_paquete* paquete);
	
	t_proceso * crearProceso(uint32_t tamanioProceso, uint32_t sizeInstrucciones,t_instruccion* instrucciones);
	void * serializarProceso(void* stream, void* estructura);
	t_proceso * deserializarProceso(void* stream);
	
	t_traduccion_direcciones * crearTraduccionDirecciones(uint32_t PID);
	void * serializarTraduccionDirecciones(void* stream, void* estructura);
	t_traduccion_direcciones * deserializarTraduccionDirecciones(void* stream);
	
#endif