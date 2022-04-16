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

t_proceso * crearProceso(uint32_t tamanioProceso, uint32_t longitudInstrucciones, char * instrucciones){
	t_proceso * proceso = malloc(sizeof(t_proceso));
	proceso->tamanioProceso = tamanioProceso;
	proceso->longitudInstrucciones = longitudInstrucciones;
	proceso->instrucciones = instrucciones;
	return proceso;
}
void * serializarProceso(void* stream, void* estructura){
    t_proceso* proceso = (t_proceso*) estructura;
    int offset = 0;
    memcpy(stream + offset, &(proceso->tamanioProceso),sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &(proceso->longitudInstrucciones),sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, proceso->instrucciones, proceso->longitudInstrucciones);
	return stream;
}

t_proceso * deserializarProceso(void* stream){
    t_proceso* proceso = malloc(sizeof(t_proceso));
    int offset = 0;
    memcpy(&(proceso->tamanioProceso), stream + offset,sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&(proceso->longitudInstrucciones), stream + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	proceso->instrucciones = malloc(sizeof(char)*(proceso->longitudInstrucciones));
	memcpy(proceso->instrucciones, stream + offset, proceso->longitudInstrucciones);
	return proceso;
}

void* serializarEstructura(void* estructura,int tamanio,t_cod_op codigoOperacion){

	void* stream = malloc(tamanio);

	switch(codigoOperacion){
		case PROCESO:
			return serializarProceso(stream,estructura);
		default:
			//printf("\n No pusiste el tipo de estructura para poder serializar negro \n");
			exit(1);
	}

}

int tamanioEstructura(void* estructura ,t_cod_op cod_op){

	switch(cod_op){

		case PROCESO:
		{
			t_proceso * proceso = (t_proceso *) estructura;
			return sizeof(uint32_t)*2 + proceso->longitudInstrucciones + 1;
		}

		default:
				//printf("\n No pusiste el tipo de estructura para ver el tamanio negro \n");
				exit(1);
	}

}

