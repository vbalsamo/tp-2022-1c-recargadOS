#ifndef STATIC_SERIALIZACION_ESTRUCTURAS_H_
#define STATIC_SERIALIZACION_ESTRUCTURAS_H_
	typedef enum{
		PROCESO,
		ALGO
	}t_cod_op;

	typedef struct{
		u_int32_t tamanioProceso;
		u_int32_t longitudInstrucciones;
		char* instrucciones;
	}t_proceso;

	void * serializarProceso(void* stream, void* estructura);

	void* serializarEstructura(void* estructura,int tamanio,t_cod_op codigoOperacion);

	int tamanioEstructura(void* estructura ,tipoDeDato cod_op);
#endif