#include<serializacion/estructuras.h>

void crearBuffer(t_paquete* paquete)
{
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
}

t_paquete* crearPaquete(t_cod_op cod_op)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = cod_op;
	crearBuffer(paquete);
	return paquete;
}


t_paquete* armarPaqueteCon(void* estructura,t_cod_op cod_op){

	t_paquete* paquete = crearPaquete(cod_op);
	paquete->buffer->size = tamanioEstructura(estructura,cod_op);
	paquete->buffer->stream = serializarEstructura(estructura,paquete->buffer->size,cod_op);

	//printf("Paquete %d creado \n", paquete->codigoOperacion);
	return  paquete;

}

void* serializarPaquete(t_paquete* paquete, uint32_t bytes)
{
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}

void agregar_a_paquete(t_paquete* paquete, void* valor, uint32_t tamanio)
{
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));

	memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(uint32_t));
	memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(uint32_t), valor, tamanio);

	paquete->buffer->size += tamanio + sizeof(uint32_t);
}

void enviarPaquete(t_paquete* paquete, int socket_cliente)
{
	uint32_t bytes = paquete->buffer->size + 2*sizeof(uint32_t);
	void* a_enviar = serializarPaquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);
	free(a_enviar);
}

void eliminarPaquete(t_paquete* paquete)
{
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

t_proceso * crearProceso(uint32_t tamanioProceso, uint32_t sizeInstrucciones, t_instruccion * instrucciones){
	t_proceso * proceso = malloc(sizeof(t_proceso));
	proceso->tamanioProceso = tamanioProceso;
	proceso->sizeInstrucciones = sizeInstrucciones;
	proceso->instrucciones = instrucciones;
	return proceso;
}
void * serializarProceso(void* stream, void* estructura){
    t_proceso* proceso = (t_proceso*) estructura;
    int offset = 0;
    memcpy(stream + offset, &proceso->tamanioProceso,sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &proceso->sizeInstrucciones,sizeof(uint32_t));
	/*
	offset += sizeof(uint32_t);
	
	memcpy(stream + offset, proceso->instrucciones, sizeof(t_instruccion)*(proceso->sizeInstrucciones));
	return stream;
	
	for(int i=0; i<proceso->sizeInstrucciones; i++){
		memcpy(stream + offset, &((proceso->instrucciones + i)->identificador), sizeof(instruccion_id));
		offset += sizeof(instruccion_id);
		memcpy(stream + offset, &((proceso->instrucciones + i)->parametro1), sizeof(uint32_t));
		offset += sizeof(uint32_t);
		memcpy(stream + offset, &((proceso->instrucciones + i)->parametro2), sizeof(uint32_t));
		offset += sizeof(uint32_t);
		//printf("\n%d,%d,%d \n",(proceso->instrucciones + i)->identificador,(proceso->instrucciones + i)->parametro1,(proceso->instrucciones + i)->parametro2);
	}
	*/
	serializarInstrucciones(stream + offset, (void*)proceso->instrucciones);
	for(int i=0; i<proceso->sizeInstrucciones; i++){
		printf("\ninstruccion_id:%d, instruccion_string:%s, parametro1:%d, parametro2:%d\n",(proceso->instrucciones + i)->identificador, instruccion_idAString((proceso->instrucciones + i)->identificador), (proceso->instrucciones + i)->parametro1, (proceso->instrucciones + i)->parametro2);
	}
	return stream;
}

t_proceso * deserializarProceso(void* stream){
    t_proceso* proceso = malloc(sizeof(t_proceso));
    //int offset = 0;
    memcpy(&(proceso->tamanioProceso), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&(proceso->sizeInstrucciones), stream, sizeof(uint32_t));
	//stream += sizeof(uint32_t);

	//proceso->instrucciones = malloc(sizeof(t_instruccion)*(proceso->sizeInstrucciones));
	/*
	memcpy(proceso->instrucciones, stream, proceso->sizeInstrucciones);
	*/
	proceso->instrucciones = deserializarInstrucciones(stream);
	return proceso;
}
void * serializarInstrucciones(void* stream, void* estructura){
	uint32_t sizeInstrucciones;
	memcpy(&(sizeInstrucciones), stream, sizeof(uint32_t));
	t_instruccion* instrucciones = (t_instruccion*) estructura;

	for(int i=0; i<sizeInstrucciones; i++){
		memcpy(stream, &((instrucciones + i)->identificador), sizeof(instruccion_id));
		stream += sizeof(instruccion_id);
		memcpy(stream, &((instrucciones + i)->parametro1), sizeof(uint32_t));
		stream += sizeof(uint32_t);
		memcpy(stream, &((instrucciones + i)->parametro2), sizeof(uint32_t));
		stream += sizeof(uint32_t);
	}

	return stream;
}
t_instruccion * deserializarInstrucciones(void * stream){
	uint32_t sizeInstrucciones;
	memcpy(&(sizeInstrucciones), stream, sizeof(uint32_t));
	t_instruccion* instrucciones = malloc(sizeof(t_instruccion)*sizeInstrucciones);

	for(int i=0; i<sizeInstrucciones; i++){
		memcpy(&((instrucciones + i)->identificador), stream, sizeof(instruccion_id));
		stream += sizeof(instruccion_id);
		memcpy(&((instrucciones + i)->parametro1), stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);
		memcpy(&((instrucciones + i)->parametro2), stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);
	}
	return instrucciones;
}

void * serializarTraduccionDirecciones(void* stream, void* estructura){
	t_traduccion_direcciones* traduccion_direcciones = (t_traduccion_direcciones*) stream;
    int offset = 0;
    memcpy(stream + offset, &(traduccion_direcciones->tamanio_pagina),sizeof(uint32_t));
	offset = sizeof(uint32_t);
    memcpy(stream + offset, &(traduccion_direcciones->paginas_por_tabla),sizeof(uint32_t));
	return stream;
}

t_traduccion_direcciones * deserializarTraduccionDirecciones(void* stream){
	t_traduccion_direcciones* traduccion_direcciones = malloc(sizeof(t_traduccion_direcciones));
    int offset = 0;
    memcpy(&(traduccion_direcciones->tamanio_pagina), stream + offset,sizeof(uint32_t));
	offset = sizeof(uint32_t);
	memcpy(&(traduccion_direcciones->paginas_por_tabla), stream + offset,sizeof(uint32_t));
	return traduccion_direcciones;
}

void* serializarEstructura(void* estructura,int tamanio,t_cod_op cod_op){

	void* stream = malloc(tamanio);

	switch(cod_op){
		case PROCESO:{
			return serializarProceso(stream,estructura);
			break;
		}
		case REQ_TRADUCCION_DIRECCIONES:{
			return NULL;
			break;
		}
		case RES_TRADUCCION_DIRECCIONES:{
			return serializarTraduccionDirecciones(stream,estructura);
			break;
		}
		default:
			fprintf(stderr,"Código de operacion %d no contemplado", cod_op);
			exit(EXIT_FAILURE);
	}

}

int tamanioEstructura(void* estructura ,t_cod_op cod_op){

	switch(cod_op){

		case PROCESO:{
			t_proceso * proceso = (t_proceso *) estructura;
			return sizeof(uint32_t)*2 + proceso->sizeInstrucciones*(sizeof(uint32_t)*2 + sizeof(instruccion_id));
			break;
		}
		case REQ_TRADUCCION_DIRECCIONES:{
			return 0;
			break;
		}
		case RES_TRADUCCION_DIRECCIONES:{
			return sizeof(uint32_t);
			break;
		}
		default:
			fprintf(stderr,"Código de operacion %d no contemplado", cod_op);
			exit(EXIT_FAILURE);
	}

}

