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
    memcpy(stream + offset, &(proceso->tamanioProceso),sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &(proceso->sizeInstrucciones),sizeof(uint32_t));
	offset += sizeof(uint32_t);
	for(int i=0; i<proceso->sizeInstrucciones; i++){
		memcpy(stream + offset, &(proceso->instrucciones[i]), sizeof(t_instruccion));
		printf("\n%d,%d,%d \n",proceso->instrucciones[i].identificador,proceso->instrucciones[i].parametro1,proceso->instrucciones[i].parametro2);
		offset += sizeof(t_instruccion);
	}
	
	return stream;
}

t_proceso * deserializarProceso(void* stream){
    t_proceso* proceso = malloc(sizeof(t_proceso));
    int offset = 0;
    memcpy(&(proceso->tamanioProceso), stream + offset,sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&(proceso->sizeInstrucciones), stream + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	proceso->instrucciones = malloc(sizeof(t_instruccion)*(proceso->sizeInstrucciones));
	for(int i=0; i<proceso->sizeInstrucciones; i++){
		memcpy(&(proceso->instrucciones[i]),stream + offset , sizeof(t_instruccion));
		printf("\n%d,%d,%d \n",proceso->instrucciones[i].identificador,proceso->instrucciones[i].parametro1,proceso->instrucciones[i].parametro2);
		offset += sizeof(t_instruccion*);
	}
	
	return proceso;
}
void *  serializarTraduccionDirecciones(void* stream, void* estructura){
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
			return sizeof(uint32_t)*2 + proceso->sizeInstrucciones*(sizeof(t_instruccion));
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

