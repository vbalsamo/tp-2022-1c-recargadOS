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
		REQ_PCB_A_EJECUTAR_KERNEL_CPU,
		PCB_EJECUTADO_IO_CPU_KERNEL,
		PCB_EJECUTADO_EXIT_CPU_KERNEL,
		PCB_EJECUTADO_INTERRUPCION_CPU_KERNEL,
		REQ_INTERRUPCION_KERNEL_CPU, //HILO INTERRUPT
		RES_INTERRUPCION_CPU_KERNEL, //HILO DISPATCH
		REQ_TRADUCCION_DIRECCIONES_CPU_MEMORIA,
		RES_TRADUCCION_DIRECCIONES_MEMORIA_CPU,
		REQ_DATO_DIRECCION_LOGICA_CPU_MEMORIA,
		REQ_READ_CPU_MEMORIA,
		RES_READ_MEMORIA_CPU,
		REQ_WRITE_CPU_MEMORIA,
		RES_WRITE_CPU_MEMORIA,
		REQ_MARCO_ESCRITURA_CPU_MEMORIA,
		REQ_MARCO_LECTURA_CPU_MEMORIA,
		RES_MARCO_MEMORIA_CPU,
		REQ_CREAR_PROCESO_KERNEL_MEMORIA,
		RES_CREAR_PROCESO_KERNEL_MEMORIA,
		REQ_TABLA_SEGUNDO_NIVEL_CPU_MEMORIA,
		RES_TABLA_SEGUNDO_NIVEL_MEMORIA_CPU,
		RES_FIN_PROCESO_KERNEL_CONSOLA,
		REQ_FIN_PROCESO_KERNEL_MEMORIA,
		REQ_SUSP_PROCESO_KERNEL_MEMORIA,
		RES_SUSP_PROCESO_KERNEL_MEMORIA,
		REQ_DESUSP_PROCESO_KERNEL_MEMORIA,
		ALGO
	}t_cod_op;

	
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
		uint32_t longitud;
		char * texto;
	}t_mensaje;
	
	typedef struct{
		uint32_t id;
		uint32_t tamanioProceso;
		uint32_t PC;
		uint32_t tablaDePaginas;
		uint32_t estimacionRafaga;
		uint32_t lengthUltimaRafaga;
		uint32_t sizeInstrucciones;
		t_instruccion* instrucciones;
	}t_pcb;

	typedef struct{
        t_pcb * pcb;
        uint32_t tiempoBloqueo;
    }t_IO;

	typedef struct{
		t_pcb * pcb;
		uint32_t tiempoBloqueo;
		uint32_t tiempoEspera;
	}t_pcbBloqueado;
	
	typedef struct{
		uint32_t tablaDePaginas;
		uint32_t entradaPagina;
		uint32_t id;
	}t_consultaTablaPagina;
	
	typedef struct{
		uint32_t direccionFisica;
		uint32_t dato;
		uint32_t id;
	}t_peticionEscritura;
	char * codOPtoString(t_cod_op);

	void* serializarEstructura(void* estructura,int tamanio,t_cod_op codigoOperacion);
	int tamanioEstructura(void* estructura ,t_cod_op cod_op);
	
	void crearBuffer(t_paquete* paquete);

    t_paquete* crearPaquete(t_cod_op cod_op);
    t_paquete* armarPaqueteCon(void* estructura,t_cod_op cod_op);
    void* serializarPaquete(t_paquete* paquete, uint32_t bytes);
    void enviarPaquete(t_paquete* paquete, int socket_cliente);
    void eliminarPaquete(t_paquete* paquete);
	
	void * serializarInstrucciones(void* stream, void* estructura, int offset);
	t_instruccion * deserializarInstrucciones(void * stream, int offset);

	t_proceso * crearProceso(uint32_t tamanioProceso, uint32_t sizeInstrucciones,t_instruccion* instrucciones);
	void freeProceso(t_proceso * proceso);
	void * serializarProceso(void* stream, void* estructura);
	t_proceso * deserializarProceso(void* stream);
	
	t_traduccion_direcciones * crearTraduccionDirecciones(uint32_t PID);
	void * serializarTraduccionDirecciones(void* stream, void* estructura);
	t_traduccion_direcciones * deserializarTraduccionDirecciones(void* stream);
	

	t_mensaje *  deserializarMensaje(void* stream);
	void *  serializarMensaje(void* stream, void* estructura);

	void * serializarPCB(void* stream, void* estructura, int offset);
	t_pcb * deserializarPCB(void* stream, int offset);
	void freePCB(t_pcb * pcb);
	
	void *  serializarIO(void* stream, void* estructura);
	t_IO * deserializarIO(void* stream);

	void * serializarUINT32_T(void* stream, void* estructura);
	uint32_t * deserializarUINT32_T(void* stream);

	t_consultaTablaPagina * deserializarConsultaTablaPagina(void* stream);
	void * serializarConsultaTablaPagina(void* stream, void* estructura);

	t_peticionEscritura * deserializarPeticionEscritura(void* stream);
	void * serializarPeticionEscritura(void* stream, void* estructura);
#endif