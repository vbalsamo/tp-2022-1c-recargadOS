#include<serializacion/estructuras.h>


void * serializarProceso(void* stream, void* estructura){
    t_proceso* proceso = (t_proceso*) estructura;
    int offset = 0;
    memcpy(stream + offset, &(proceso->tamanioProceso),sizeof(u_int32_t));
	offset += sizeof(u_int32_t);
	memcpy(stream + offset, &(proceso->longitudInstrucciones),sizeof(u_int32_t));
	offset += sizeof(u_int32_t);
	memcpy(stream + offset, proceso->instrucciones, proceso->longitudInstrucciones);
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

int tamanioEstructura(void* estructura ,tipoDeDato cod_op){

	switch(cod_op){

		case PROCESO:
		{
			
			return ;
		}

		default:
				//printf("\n No pusiste el tipo de estructura para ver el tamanio negro \n");
				exit(1);
	}

}