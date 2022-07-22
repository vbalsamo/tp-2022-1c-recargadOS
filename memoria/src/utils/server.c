#include <utils/server.h>

void iniciarEstructurasServer()
{

    pthread_mutex_init(&mutex_colaKernel, (void *)NULL);
    pthread_mutex_init(&mutex_colaCPU, (void *)NULL);

    sem_init(&sem_kernel, 0, 0);
    sem_init(&sem_CPU, 0, 0);

    colaKernel = queue_create();
    colaCPU = queue_create();

    pthread_t hilo_Kernel;
    pthread_create(&hilo_Kernel, NULL, (void *)hiloKernel, NULL);
    pthread_detach(hilo_Kernel);

    pthread_t hilo_CPU;
    pthread_create(&hilo_CPU, NULL, (void *)hiloCPU, NULL);
    pthread_detach(hilo_CPU);
}
void atenderPeticion(int *socket_cliente)
{
    t_paquete *paquete = recibirPaquete(*socket_cliente);

    t_operacion *operacion = malloc(sizeof(t_operacion));
    operacion->paquete = paquete;
    operacion->socket = socket_cliente;
    if (paqueteDeKernel(paquete))
    {
        pthread_mutex_lock(&mutex_colaKernel);
        queue_push(colaKernel, operacion);
        pthread_mutex_unlock(&mutex_colaKernel);
        sem_post(&sem_kernel);
    }
    else
    {
        pthread_mutex_lock(&mutex_colaCPU);
        queue_push(colaCPU, operacion);
        pthread_mutex_unlock(&mutex_colaCPU);
        sem_post(&sem_CPU);
    }
}
bool paqueteDeKernel(t_paquete *paquete)
{
    t_cod_op codigo = paquete->codigo_operacion;
    return codigo == REQ_CREAR_PROCESO_KERNEL_MEMORIA || codigo == REQ_SUSP_PROCESO_KERNEL_MEMORIA || codigo == REQ_FIN_PROCESO_KERNEL_MEMORIA;
}
void hiloKernel()
{
    while (true)
    {
        sem_wait(&sem_kernel);
        pthread_mutex_lock(&mutex_colaKernel);
        t_operacion *operacion = queue_pop(colaKernel);
        pthread_mutex_unlock(&mutex_colaKernel);
        deserializarPeticionKernel(operacion->paquete, *operacion->socket);
        free(operacion->socket);
        free(operacion);
    }
}
void hiloCPU()
{
    while (true)
    {
        sem_wait(&sem_CPU);
        pthread_mutex_lock(&mutex_colaCPU);
        t_operacion *operacion = queue_pop(colaCPU);
        pthread_mutex_unlock(&mutex_colaCPU);
        deserializarPeticionCPU(operacion->paquete, *operacion->socket);
        free(operacion->socket);
        free(operacion);
    }
}
void deserializarPeticionKernel(t_paquete *paquete, int socket)
{
    switch (paquete->codigo_operacion)
    {
    case REQ_CREAR_PROCESO_KERNEL_MEMORIA:
    {
        t_pcb *pcb = deserializarPCB(paquete->buffer->stream, 0);
        log_info(logger, "se solicita crear estructuras del proceso %d", pcb->id);
        uint32_t *tablaPaginas1erNivel = malloc(sizeof(uint32_t));
        *tablaPaginas1erNivel = inicializarEstructurasProceso(pcb);
        t_paquete *paqueteRespuesta = armarPaqueteCon(tablaPaginas1erNivel, RES_CREAR_PROCESO_KERNEL_MEMORIA);
        crearArchivoSwap(pcb->id, pcb->tamanioProceso);
        enviarPaquete(paqueteRespuesta, socket);
        eliminarPaquete(paqueteRespuesta);
        log_info(logger, "se envia el índice de la tabla de páginas del proceso %d a kernel", pcb->id);
        free(tablaPaginas1erNivel);
        freePCB(pcb);
        break;
    }
    case REQ_SUSP_PROCESO_KERNEL_MEMORIA:
    {
        t_pcb *pcb = deserializarPCB(paquete->buffer->stream, 0);
        log_info(logger, " ");
        log_info(logger, "se solicita suspender el proceso %d, se swapean sus páginas", pcb->id);
        log_info(logger, " ");
        suspenderProceso(pcb); // liberarMarcos()
        eliminarMarcos(pcb->tablaDePaginas);
        uint32_t respuesta = 1;
        t_paquete *paqueteRespuesta = armarPaqueteCon(&respuesta, RES_SUSP_PROCESO_KERNEL_MEMORIA);
        enviarPaquete(paqueteRespuesta, socket);
        eliminarPaquete(paqueteRespuesta);
        log_info(logger, "Finaliza la suspensión del proceso: %d", pcb->id);
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
    default:
    {
        perror("codigo de operacion no valido para KERNEL");
        exit(EXIT_FAILURE);
    }
    }
    eliminarPaquete(paquete);
    close(socket);
}
void deserializarPeticionCPU(t_paquete *paquete, int socket)
{

    switch (paquete->codigo_operacion)
    {
    case REQ_TRADUCCION_DIRECCIONES_CPU_MEMORIA:
    {
        t_traduccion_direcciones *traduccionDirecciones = malloc(sizeof(t_traduccion_direcciones));
        traduccionDirecciones->tamanio_pagina = TAM_PAGINA;
        traduccionDirecciones->paginas_por_tabla = ENTRADAS_POR_TABLA;
        t_paquete *paqueteRespuesta = armarPaqueteCon(traduccionDirecciones, RES_TRADUCCION_DIRECCIONES_MEMORIA_CPU);
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
        log_info(logger, "se asigna el marco: %d al pcb: %d. LECTURA", marco, consulta->id);
        enviarPaquete(paqueteRespuesta, socket);
        eliminarPaquete(paqueteRespuesta);
        free(consulta);
        break;
    }
    case REQ_MARCO_ESCRITURA_CPU_MEMORIA:
    {
        t_consultaTablaPagina *consulta = deserializarConsultaTablaPagina(paquete->buffer->stream);
        uint32_t marco = obtenerMarco(consulta->tablaDePaginas, consulta->entradaPagina, consulta->id, true);
        log_info(logger, "se asigna el marco: %d al pcb: %d. ESCRITURA", marco, consulta->id);
        t_paquete *paqueteRespuesta = armarPaqueteCon(&marco, RES_MARCO_MEMORIA_CPU);
        enviarPaquete(paqueteRespuesta, socket);
        eliminarPaquete(paqueteRespuesta);
        free(consulta);
        break;
    }
    case REQ_READ_CPU_MEMORIA:
    {
        uint32_t *direccionFisica = deserializarUINT32_T(paquete->buffer->stream);
        uint32_t *datoMemoria = leerDireccionFisica(*direccionFisica);
        t_paquete *paqueteRespuesta = armarPaqueteCon(datoMemoria, RES_READ_MEMORIA_CPU);
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
        enviarPaquete(paqueteRespuesta, socket);
        eliminarPaquete(paqueteRespuesta);
        free(aEscribir);
        break;
    }
    default:
    {
        perror("codigo de operacion no valido para CPU");
        exit(EXIT_FAILURE);
    }
    }
    eliminarPaquete(paquete);
    close(socket);
}