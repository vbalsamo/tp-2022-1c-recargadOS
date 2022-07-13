#include <utils/server.h>

void deserializarSegun(t_paquete *paquete, int socket)
{
    switch (paquete->codigo_operacion)
    {
        case REQ_CREAR_PROCESO_KERNEL_MEMORIA:
        {
            log_info(logger, "se solicita crear proceso");

            t_pcb *pcb = deserializarPCB(paquete->buffer->stream, 0);
            uint32_t *tablaPaginas1erNivel = malloc(sizeof(uint32_t));
            *tablaPaginas1erNivel = inicializarEstructurasProceso(pcb);
            t_paquete *paqueteRespuesta = armarPaqueteCon(tablaPaginas1erNivel, RES_CREAR_PROCESO_KERNEL_MEMORIA);
            crearArchivoSwap(pcb->id, pcb->tamanioProceso);
            enviarPaquete(paqueteRespuesta, socket);
            eliminarPaquete(paqueteRespuesta);
            log_info(logger, "se envia tabla Paginas 1er Nivel");
            free(tablaPaginas1erNivel);
            freePCB(pcb);
            break;
        }
        case REQ_SUSP_PROCESO_KERNEL_MEMORIA:
        {
            t_pcb *pcb = deserializarPCB(paquete->buffer->stream, 0);
            suspenderProceso(pcb); // liberarMarcos()
            eliminarMarcos(pcb->tablaDePaginas);
            log_info(logger, "se solicita suspender el proceso, se swapean sus marcos: %d", pcb->id);
            freePCB(pcb);
            break;
        }
        case REQ_FIN_PROCESO_KERNEL_MEMORIA:
        {
            t_pcb *pcb = deserializarPCB(paquete->buffer->stream, 0);
            eliminarMarcos(pcb->tablaDePaginas);
            eliminarArchivoSwap(pcb->id);
            log_info(logger, "se solicita borrar memoria y swap del proceso: %d", pcb->id);
            freePCB(pcb);
            break;
        }
        case REQ_TRADUCCION_DIRECCIONES_CPU_MEMORIA:
        {
            log_info(logger, "se solicita traducciones de direciones");
            t_traduccion_direcciones *traduccionDirecciones = malloc(sizeof(t_traduccion_direcciones));

            traduccionDirecciones->tamanio_pagina = TAM_PAGINA;
            traduccionDirecciones->paginas_por_tabla = ENTRADAS_POR_TABLA;
            t_paquete *paqueteRespuesta = armarPaqueteCon(traduccionDirecciones, RES_TRADUCCION_DIRECCIONES_MEMORIA_CPU);
            retardoMemoria();
            enviarPaquete(paqueteRespuesta, socket);
            log_info(logger, "se envia traducciones de direciones");
            free(traduccionDirecciones);
            eliminarPaquete(paqueteRespuesta);
            break;
        }
        case REQ_TABLA_SEGUNDO_NIVEL_CPU_MEMORIA:
        {
            t_consultaTablaPagina *consulta = deserializarConsultaTablaPagina(paquete->buffer->stream);
            uint32_t tablaSegundoNivel = obtenerTablaSegundoNivel(consulta->tablaDePaginas, consulta->entradaPagina);
            t_paquete *paqueteRespuesta = armarPaqueteCon(&tablaSegundoNivel, RES_TABLA_SEGUNDO_NIVEL_MEMORIA_CPU);
            retardoMemoria();
            enviarPaquete(paqueteRespuesta, socket);
            eliminarPaquete(paqueteRespuesta);
            free(consulta);
            break;
        }
        case REQ_MARCO_LECTURA_CPU_MEMORIA:
        {
            t_consultaTablaPagina *consulta = deserializarConsultaTablaPagina(paquete->buffer->stream);
            uint32_t marco = obtenerMarco(consulta->tablaDePaginas, consulta->entradaPagina, consulta->id, false);
            t_paquete *paqueteRespuesta = armarPaqueteCon(&marco, RES_MARCO_MEMORIA_CPU);
            retardoMemoria();
            enviarPaquete(paqueteRespuesta, socket);
            eliminarPaquete(paqueteRespuesta);
            free(consulta);
            break;
        }
        case REQ_MARCO_ESCRITURA_CPU_MEMORIA:
        {
            t_consultaTablaPagina *consulta = deserializarConsultaTablaPagina(paquete->buffer->stream);
            uint32_t marco = obtenerMarco(consulta->tablaDePaginas, consulta->entradaPagina, consulta->id, true);
            t_paquete *paqueteRespuesta = armarPaqueteCon(&marco, RES_MARCO_MEMORIA_CPU);
            retardoMemoria();
            enviarPaquete(paqueteRespuesta, socket);
            eliminarPaquete(paqueteRespuesta);
            free(consulta);
            break;
        }
        case REQ_READ_CPU_MEMORIA:
        {
            uint32_t *direccionFisica = deserializarUINT32_T(paquete->buffer->stream);
            uint32_t *datoMemoria = leerDireccionFisica(*direccionFisica);
            log_info(logger, "se lee el dato: %d en el direccion Fisica: %d", *datoMemoria, *direccionFisica);
            t_paquete *paqueteRespuesta = armarPaqueteCon(datoMemoria, RES_READ_MEMORIA_CPU);
            retardoMemoria();
            enviarPaquete(paqueteRespuesta, socket);
            eliminarPaquete(paqueteRespuesta);
            free(direccionFisica);
            free(datoMemoria);
            break;
        }
        case REQ_WRITE_CPU_MEMORIA:
        {
            t_peticionEscritura *aEscribir = deserializarPeticionEscritura(paquete->buffer->stream);
            writeEnMemoria(aEscribir->direccionFisica, aEscribir->dato);
            // responder ok escritura
            uint32_t respuesta = 1;
            t_paquete *paqueteRespuesta = armarPaqueteCon(&respuesta, RES_WRITE_CPU_MEMORIA);
            retardoMemoria();
            enviarPaquete(paqueteRespuesta, socket);
            eliminarPaquete(paqueteRespuesta);
            free(aEscribir);
            break;
        }
        default:
        {
            break;
        }
    }
    eliminarPaquete(paquete);
    close(socket);
}
