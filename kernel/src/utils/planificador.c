#include <utils/planificador.h>
int id_nuevo_proceso = 0;

/*
t_proceso * crearProceso(uint32_t tamanioProceso, uint32_t sizeInstrucciones, t_instruccion * instrucciones){
    t_proceso * proceso = malloc(sizeof(t_proceso));
    proceso->tamanioProceso = tamanioProceso;
    proceso->sizeInstrucciones = sizeInstrucciones;
    proceso->instrucciones = instrucciones;
    return proceso;
*/
t_pcb *iniciarPcb(t_proceso *proceso)
{
    t_pcb *pcb = malloc(sizeof(t_pcb));

    pcb->id = id_nuevo_proceso;
    id_nuevo_proceso++;
    pcb->tamanioProceso = proceso->tamanioProceso;
    pcb->sizeInstrucciones = proceso->sizeInstrucciones;
    pcb->instrucciones = proceso->instrucciones;
    pcb->lengthUltimaRafaga = 0;
    pcb->PC = 0;
    pcb->tablaDePaginas = 0; //¿iniciar conexion con memomoria para solicitar tabla de paginas?
    pcb->estimacionRafaga = ESTIMACION_INICIAL;
    free(proceso);
    return pcb;
}

void ingresarANew(t_pcb *pcb)
{
    pthread_mutex_lock(&mutex_estado_new);
    queue_push(estado_new, (void *)pcb);
    pthread_mutex_unlock(&mutex_estado_new);
    log_info(logger, "Se agrega el proceso:%d a new", pcb->id);
    // sem_post(&sem_hay_pcb_en_new);
    sem_post(&sem_hay_pcb_esperando_ready);
}

void Aready()
{
    while (1)
    {

        sem_wait(&sem_hay_pcb_esperando_ready);

        sem_wait(&sem_multiprogramacion);
        log_info(logger, "Grado de multiprogramación permite agregar proceso a ready\n");

        t_pcb *pcb = obtenerSiguienteAready();

        addEstadoReady(pcb);
        log_info(logger, "Se elimino el proceso %d de New y se agrego a Ready", pcb->id);

        if (string_equals_ignore_case(ALGORITMO_PLANIFICACION, "SRT"))
        {
            interrumpirPCB();
        }
        sem_post(&sem_ready);
    }
}

t_pcb *obtenerSiguienteAready()
{

    t_pcb *pcb = NULL;

    if (!queue_is_empty(estado_susp_ready))
    {

        pthread_mutex_lock(&mutex_estado_susp_ready);
        pcb = queue_pop(estado_susp_ready);
        pthread_mutex_unlock(&mutex_estado_susp_ready);
        // comunicacionMemoriaDesuspender(pcb);
        // por que hacemos esto? no sabemos que paginas necesitariamos
        // levantar del swap para colocar en memoria
        // al pasar de susp a ready no debemos mandar este mensaje
        return pcb;
    }
    else
    {
        pthread_mutex_lock(&mutex_estado_new);
        pcb = queue_pop(estado_new);
        comunicacionMemoriaCreacionEstructuras(pcb);
        pthread_mutex_unlock(&mutex_estado_new);
        return pcb;
    }
}

void readyAexec()
{
    // hilo de ejecucion
    while (1)
    {
        sem_wait(&sem_ready);
        t_pcb *pcb;
        pcb = algoritmoPlanificacion();
        bool filtro(void *pcbAux)
        {
            return ((t_pcb *)pcbAux)->id == pcb->id;
        };
        pthread_mutex_lock(&mutex_estado_ready);
        list_remove_by_condition(estado_ready, filtro);
        pthread_mutex_unlock(&mutex_estado_ready);
        id_pcb_ejecutando = pcb->id;
        ejecutarPCB(pcb);
    }
}
// pcb->estimacionRafaga = alfa*pcb->lengthUltimaRafaga + (1-alfa)*pcb->estimacionRafaga

t_pcb *planificacionSRT()
{

    t_pcb *pcbPlanificado = NULL;
    t_pcb *pcbAux = NULL;
    int i;
    int indexAPlanificar;
    float shortestJob;

    pthread_mutex_lock(&mutex_estado_ready);
    pcbAux = list_get(estado_ready, 0);
    pthread_mutex_unlock(&mutex_estado_ready);

    indexAPlanificar = 0;
    shortestJob = pcbAux->estimacionRafaga;

    pthread_mutex_lock(&mutex_estado_ready);

    log_info(logger, "PCBS EN READY: %d", list_size(estado_ready));

    t_pcb *pcbFor = NULL;
    for (i = 0; i < list_size(estado_ready); i++)
    {
        pcbFor = list_get(estado_ready, i);
        log_info(logger, "id: %d | estimacionRafaga: %d | lenghtUltimaRafaga: %d", pcbFor->id, pcbFor->estimacionRafaga, pcbFor->lengthUltimaRafaga);
    }

    pthread_mutex_lock(&mutex_estado_susp_ready);
    log_info(logger, "PCBS EN SUSP_READY: %d\n", queue_size(estado_susp_ready));
    pthread_mutex_unlock(&mutex_estado_susp_ready);
    uint32_t _id_pcb_ejecutando = id_pcb_ejecutando;
    bool filtro(void *pcbFiltro)
    {
        return ((t_pcb *)pcbFiltro)->id == _id_pcb_ejecutando;
    };

    t_pcb *pcb_en_ejecucion = list_find(estado_ready, filtro);
    if (pcb_en_ejecucion == NULL)
    {
        pcbPlanificado = list_get(estado_ready, indexAPlanificar);
        pthread_mutex_unlock(&mutex_estado_ready);
        return pcbPlanificado;
    }

    for (i = 1; i < list_size(estado_ready); i++)
    {
        pcbAux = list_get(estado_ready, i);

        if (shortestJob > pcbAux->estimacionRafaga)
        {
            shortestJob = pcbAux->estimacionRafaga;
            indexAPlanificar = i;
        }
    }

    if (shortestJob < (pcb_en_ejecucion->estimacionRafaga))
    {
        log_info(logger, "SRT: pcb en ready es mas prioritario que el de exec");
        pcbPlanificado = list_get(estado_ready, indexAPlanificar);
        pthread_mutex_unlock(&mutex_estado_ready);
        return pcbPlanificado;
    }
    else
    {
        pthread_mutex_unlock(&mutex_estado_ready);
        return pcb_en_ejecucion;
    }

    pcbPlanificado = list_get(estado_ready, indexAPlanificar);
    pthread_mutex_unlock(&mutex_estado_ready);
    return pcbPlanificado;
}

void execAexit(t_pcb *pcb)
{
    bool filtro(void *consola)
    {
        return ((t_consola *)consola)->id == pcb->id;
    };
    t_consola *consolaAnotificar;

    comunicacionMemoriaFinalizar(pcb);

    pthread_mutex_lock(&mutex_consolas_conectadas);
    consolaAnotificar = list_remove_by_condition(consolas_conectadas, filtro);
    pthread_mutex_unlock(&mutex_consolas_conectadas);

    t_paquete *paqueteAconsola = armarPaqueteCon(&(pcb->id), RES_FIN_PROCESO_KERNEL_CONSOLA);
    enviarPaquete(paqueteAconsola, *consolaAnotificar->socket);
    eliminarPaquete(paqueteAconsola);

    close(*consolaAnotificar->socket);
    free(consolaAnotificar->socket);
    free(consolaAnotificar);
    freePCB(pcb);
    sem_post(&sem_multiprogramacion);
}

void sumarEspera(void *pcbBloqueado)
{
    t_pcbBloqueado *aModificar = (t_pcbBloqueado *)pcbBloqueado;
    aModificar->tiempoEspera += (100 * 1000);
}

void sumarEsperaSegundos(void *pcbBloqueado)
{
    t_pcbBloqueado *aModificar = (t_pcbBloqueado *)pcbBloqueado;
    aModificar->tiempoEspera += (1000000);
}

void hacerIO(uint32_t tiempoBloqueo)
{
    log_info(logger, "haciendo IO de %d milisegundos", tiempoBloqueo / 1000);
    if (tiempoBloqueo < 1000000)
    {
        for (int i = 0; i < tiempoBloqueo; i += (100 * 1000))
        {
            usleep(100 * 1000); // 100 milisegundos
            pthread_mutex_lock(&mutex_estado_blocked);
            list_iterate(estado_blocked->elements, sumarEspera);
            pthread_mutex_unlock(&mutex_estado_blocked);
        }
    }
    else
    {
        for (int i = 0; i < tiempoBloqueo / 1000000; i++)
        {
            sleep(1); // 100 milisegundos
            pthread_mutex_lock(&mutex_estado_blocked);
            list_iterate(estado_blocked->elements, sumarEsperaSegundos);
            pthread_mutex_unlock(&mutex_estado_blocked);
        }
    }
}

void hilo_block()
{
    while (1)
    {
        sem_wait(&sem_block);
        pthread_mutex_lock(&mutex_estado_blocked);
        t_pcbBloqueado *ultimoPcbBloqueado = queue_pop(estado_blocked);
        pthread_mutex_unlock(&mutex_estado_blocked);
        uint32_t bloqueo = ultimoPcbBloqueado->tiempoEspera + (ultimoPcbBloqueado->tiempoBloqueo * 1000);

        if (bloqueo > TIEMPO_MAXIMO_BLOQUEADO)
        {
            int ioParaSuspender = TIEMPO_MAXIMO_BLOQUEADO - ultimoPcbBloqueado->tiempoEspera;
            if (ioParaSuspender < 0)
            {
                log_info(logger, "tiempo de espera del pcb: %d: %d milisegundos", ultimoPcbBloqueado->pcb->id, ultimoPcbBloqueado->tiempoEspera / 1000);
                log_info(logger, "pcb: %d ejecutando IO de: %d milisegundos", ultimoPcbBloqueado->pcb->id, ultimoPcbBloqueado->tiempoBloqueo);
                comunicacionMemoriaSuspender(ultimoPcbBloqueado->pcb);
                sem_post(&sem_multiprogramacion);
                hacerIO(ultimoPcbBloqueado->tiempoBloqueo * 1000);
                log_info(logger, "pcb: %d excedió el tiempo maximo de bloqueo permitido de %d milisegundos", ultimoPcbBloqueado->pcb->id, TIEMPO_MAXIMO_BLOQUEADO / 1000);
                log_info(logger, "Se suspende el pcb");
                addEstadoSuspReady(ultimoPcbBloqueado->pcb);
                sem_post(&sem_hay_pcb_esperando_ready);
            }
            else
            {
                log_info(logger, "tiempo de espera del pcb: %d: %d milisegundos", ultimoPcbBloqueado->pcb->id, ultimoPcbBloqueado->tiempoEspera / 1000);
                log_info(logger, "pcb: %d ejecutando IO de: %d milisegundos", ultimoPcbBloqueado->pcb->id, ioParaSuspender / 1000);
                hacerIO(ioParaSuspender);
                log_info(logger, "pcb: %d excedió el tiempo maximo de bloqueo permitido de %d milisegundos", ultimoPcbBloqueado->pcb->id, TIEMPO_MAXIMO_BLOQUEADO / 1000);
                log_info(logger, "Se suspende el pcb");
                comunicacionMemoriaSuspender(ultimoPcbBloqueado->pcb);
                sem_post(&sem_multiprogramacion);
                log_info(logger, "pcb: %d ejecutando IO restante de %d milisegundos", ultimoPcbBloqueado->pcb->id, (ultimoPcbBloqueado->tiempoBloqueo - ioParaSuspender / 1000));
                hacerIO(ultimoPcbBloqueado->tiempoBloqueo * 1000 - ioParaSuspender); // restante
                addEstadoSuspReady(ultimoPcbBloqueado->pcb);
                sem_post(&sem_hay_pcb_esperando_ready);
            }
        }
        else
        {
            hacerIO(ultimoPcbBloqueado->tiempoBloqueo);
            log_info(logger, "terminó la io del proceso: %d", ultimoPcbBloqueado->pcb->id);
            addEstadoReady(ultimoPcbBloqueado->pcb);
            if (string_equals_ignore_case(ALGORITMO_PLANIFICACION, "SRT"))
            {
                interrumpirPCB();
            }
            sem_post(&sem_ready);
        }
        free(ultimoPcbBloqueado);
        // if(ultimoPcbBloqueado->tiempoEspera>TIEMPO_MAXIMO_BLOQUEADO && !list_find(pcb))

        // uint32_t ultimoIOenSegundos = ultimoPcbBloqueado->tiempoBloqueo/1000;
        // log_info(logger, "pcb: %d ejecutando IO de: %d segundos", ultimoPcbBloqueado->pcb->id, ultimoIOenSegundos);
        // if(ultimoIOenSegundos > TIEMPO_MAXIMO_BLOQUEADO){
        //     log_info(logger, "pcb: %d excedió el tiempo maximo de IO permitido de %d segundos", ultimoPcbBloqueado->pcb->id, TIEMPO_MAXIMO_BLOQUEADO);
        //     log_info(logger, "Se suspende el pcb");
        //     sleep(TIEMPO_MAXIMO_BLOQUEADO);
        //     comunicacionMemoriaSuspender(ultimoPcbBloqueado->pcb);
        //     sem_post(&sem_multiprogramacion);
        //     log_info(logger, "pcb: %d ejecutando IO restante de %d segundos", ultimoPcbBloqueado->pcb->id, (ultimoIOenSegundos - TIEMPO_MAXIMO_BLOQUEADO));
        //     sleep(ultimoIOenSegundos - TIEMPO_MAXIMO_BLOQUEADO);
        //     log_info(logger, "terminó la io del proceso: %d", ultimoPcbBloqueado->pcb->id);
        //     addEstadoSuspReady(ultimoPcbBloqueado->pcb);
        //     sem_post(&sem_hay_pcb_esperando_ready);
        // } else{
        //     sleep(ultimoIOenSegundos);
        //     addEstadoReady(ultimoPcbBloqueado->pcb);
        //     log_info(logger, "terminó la io del proceso: %d", ultimoPcbBloqueado->pcb->id);

        //     if(string_equals_ignore_case(ALGORITMO_PLANIFICACION,"SRT")){
        //         interrumpirPCB();
        //     }
        //     sem_post(&sem_ready);
        // }
    }

} // REQ_CREAR_PROCESO_KERNEL_MEMORIA

void comunicacionMemoriaSuspender(t_pcb *pcb)
{
    int socketMemoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA);
    t_paquete *paqueteAmemoria = armarPaqueteCon(pcb, REQ_SUSP_PROCESO_KERNEL_MEMORIA);
    enviarPaquete(paqueteAmemoria, socketMemoria);
    eliminarPaquete(paqueteAmemoria);
    t_paquete *paqueteRespuesta = recibirPaquete(socketMemoria);
    if (paqueteRespuesta->codigo_operacion == RES_SUSP_PROCESO_KERNEL_MEMORIA)
    {
        log_info(logger, "Memoria terminó de suspender el proceso: %d", pcb->id);
    }
    eliminarPaquete(paqueteRespuesta);
}

void comunicacionMemoriaCreacionEstructuras(t_pcb *pcb)
{
    int socketMemoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA);
    t_paquete *paqueteAmemoria = armarPaqueteCon(pcb, REQ_CREAR_PROCESO_KERNEL_MEMORIA);
    enviarPaquete(paqueteAmemoria, socketMemoria);
    eliminarPaquete(paqueteAmemoria);
    t_paquete *paqueteRespuesta = recibirPaquete(socketMemoria);
    uint32_t *tablaPaginas1erNivel = deserializarUINT32_T(paqueteRespuesta->buffer->stream);
    pcb->tablaDePaginas = *tablaPaginas1erNivel;
    log_info(logger, "Se le asigna tabla de paginas primer nivel con index: %d al pcb: %d", *tablaPaginas1erNivel, pcb->id);
    eliminarPaquete(paqueteRespuesta);
    free(tablaPaginas1erNivel);
}

void comunicacionMemoriaFinalizar(t_pcb *pcb)
{
    int socketMemoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA);
    t_paquete *paqueteAmemoria = armarPaqueteCon(pcb, REQ_FIN_PROCESO_KERNEL_MEMORIA);
    enviarPaquete(paqueteAmemoria, socketMemoria);
    eliminarPaquete(paqueteAmemoria);
    // CONSULTAR: Esperar confirmacion de Memoria?
}

t_pcb *algoritmoPlanificacion()
{
    if (string_equals_ignore_case(ALGORITMO_PLANIFICACION, "FIFO"))
    {
        log_info(logger, "Se planifica FIFO");
        t_pcb *pcb = planificacionFIFO();
        log_info(logger, "FIFO elige el pcb: %d y se pasa a exec", pcb->id);
        return pcb;
    }
    else if (string_equals_ignore_case(ALGORITMO_PLANIFICACION, "SRT"))
    {
        log_info(logger, "Se planifica SRT");
        t_pcb *pcb = planificacionSRT();
        log_info(logger, "SRT elige el pcb: %d y se pasa a exec", pcb->id);
        return pcb;
    }
    else
    {
        char *error = string_new();
        string_append_with_format(&error, "Algoritmo de planificacion %s no soportado", ALGORITMO_PLANIFICACION);
        perror(error);
        exit(-1);
    }
}

void inicializarPlanificacion()
{

    pthread_t hilo_Aready;
    pthread_create(&hilo_Aready, NULL, (void *)Aready, NULL);
    pthread_detach(hilo_Aready);

    pthread_t hilo_readyAexec;
    pthread_create(&hilo_readyAexec, NULL, (void *)readyAexec, NULL);
    pthread_detach(hilo_readyAexec);

    pthread_t hiloblock;
    pthread_create(&hiloblock, NULL, (void *)hilo_block, NULL);
    pthread_detach(hiloblock);
}

t_pcb *planificacionFIFO()
{
    t_pcb *pcb;
    pthread_mutex_lock(&mutex_estado_ready);
    pcb = (t_pcb *)list_remove(estado_ready, 0);
    pthread_mutex_unlock(&mutex_estado_ready);

    return pcb;
}

// t_pcb * desalojarYEjecutar(t_pcb * pcb){
//     int socket_interrupt = crear_conexion(IP_CPU, PUERTO_CPU_INTERRUPT);
//     t_paquete * paquete = armarPaqueteCon(NULL, REQ_INTERRUPCION_KERNEL_CPU);
//     enviarPaquete(paquete, socket_interrupt);

//     bool filtro(void* pcbFiltro){
//         return ((t_pcb*) pcbFiltro)->id == pcb->id;
//     };

//     pthread_mutex_lock(&mutex_estado_ready);
//     t_pcb * pcb = list_remove_by_condition(lista_ready, filtro);
//     pthread_mutex_unlock(&mutex_estado_ready);
//     return pcb;
// }
void interrumpirPCB()
{
    log_info(logger, "Interrumpiendo proceso");
    int socketInterrupt = crear_conexion(IP_CPU, PUERTO_CPU_INTERRUPT);
    int numero = 1;
    t_paquete *paquete = armarPaqueteCon(&numero, REQ_INTERRUPCION_KERNEL_CPU);
    enviarPaquete(paquete, socketInterrupt);
    eliminarPaquete(paquete);
    // close(socketInterrupt);
}

void ejecutarPCB(t_pcb *pcb)
{
    int socketDispatch = crear_conexion(IP_CPU, PUERTO_CPU_DISPATCH);
    t_paquete *paquete = armarPaqueteCon(pcb, REQ_PCB_A_EJECUTAR_KERNEL_CPU);
    enviarPaquete(paquete, socketDispatch);
    eliminarPaquete(paquete);
    freePCB(pcb);

    t_paquete *paqueteRespuesta = recibirPaquete(socketDispatch);
    switch (paqueteRespuesta->codigo_operacion)
    {
    case PCB_EJECUTADO_IO_CPU_KERNEL:
    {

        t_IO *io = deserializarIO(paqueteRespuesta->buffer->stream);
        io->pcb->estimacionRafaga = ALFA * (io->pcb->lengthUltimaRafaga) + (1 - ALFA) * (io->pcb->estimacionRafaga);
        log_info(logger, "vuelve para hacer su IO: id: %d | estimacionRafaga: %d | lenghtUltimaRafaga: %d", io->pcb->id, io->pcb->estimacionRafaga, io->pcb->lengthUltimaRafaga);
        t_pcbBloqueado *pcbBloqueado = malloc(sizeof(t_pcbBloqueado));
        pcbBloqueado->pcb = io->pcb;
        pcbBloqueado->tiempoBloqueo = io->tiempoBloqueo;
        pcbBloqueado->tiempoEspera = 0;
        addEstadoBlocked(pcbBloqueado);
        sem_post(&sem_block);
        free(io);
        // t_pcb * pcbActualizado = deserializarPCB(paqueteRespuesta->buffer->stream, 0);
        // pcbActualizado->estimacionRafaga = ALFA*(io->pcb->lengthUltimaRafaga) + (1-ALFA)*(io->pcb->estimacionRafaga);
        // log_info(logger, "estimacionRafaga: %d, id: %d, lengthUltimaRafaga: %d,PC: %d, sizeInstrucciones: %d, tablaDePaginas: %d, tamanioProceso: %d",
        //         pcbActualizado->estimacionRafaga, pcbActualizado->id, pcbActualizado->lengthUltimaRafaga, pcbActualizado->PC, pcbActualizado->sizeInstrucciones, pcbActualizado->tablaDePaginas, pcbActualizado->tamanioProceso);
        // addEstadoBlocked(pcbActualizado);

        // el hilo de bloqueados es el que se bloquea con usleep(io->tiempoBloqueo)
        break;
    }
    case PCB_EJECUTADO_EXIT_CPU_KERNEL:
    {
        log_info(logger, "Entró un pcb a exit");
        t_pcb *pcbActualizado = deserializarPCB(paqueteRespuesta->buffer->stream, 0);
        execAexit(pcbActualizado);

        break;
    }
    case PCB_EJECUTADO_INTERRUPCION_CPU_KERNEL:
    {
        t_pcb *pcbActualizado = deserializarPCB(paqueteRespuesta->buffer->stream, 0);
        pcbActualizado->estimacionRafaga -= pcbActualizado->lengthUltimaRafaga;
        log_info(logger, "Entró un pcb desalojado por interrupción ID: id: %d | estimacionRafaga: %d | lenghtUltimaRafaga: %d", pcbActualizado->id, pcbActualizado->estimacionRafaga, pcbActualizado->lengthUltimaRafaga);
        addEstadoReady(pcbActualizado);
        sem_post(&sem_ready);
        // pthread_mutex_lock(&mutex_pcb_ejecutando);
        // pcbEjecutando = pcbActualizado;
        // pthread_mutex_unlock(&mutex_pcb_ejecutando)
        // sem_post(&sem_no_suspended_ready);;

        break;
    }
    default:
    {
        eliminarPaquete(paqueteRespuesta);
        char *error = string_new();
        string_append_with_format(&error, "Respuesta de CPU %s no soportada", codOPtoString(paqueteRespuesta->codigo_operacion));

        perror(error);
        exit(EXIT_FAILURE);
        break;
    }
    }
    eliminarPaquete(paqueteRespuesta);
}

void addEstadoReady(t_pcb *pcb)
{
    pthread_mutex_lock(&mutex_estado_ready);
    log_info(logger, "se agrega pcb: %d a ready", pcb->id);
    list_add(estado_ready, (void *)pcb);
    pthread_mutex_unlock(&mutex_estado_ready);
}

void addEstadoBlocked(t_pcbBloqueado *pcbBloqueado)
{
    pthread_mutex_lock(&mutex_estado_blocked);
    log_info(logger, "se agrega pcb: %d a blocked", pcbBloqueado->pcb->id);
    queue_push(estado_blocked, (void *)pcbBloqueado);
    pthread_mutex_unlock(&mutex_estado_blocked);
}

void addEstadoSuspReady(t_pcb *pcb)
{
    pthread_mutex_lock(&mutex_estado_susp_ready);
    log_info(logger, "se agrega pcb: %d a susp-ready", pcb->id);
    queue_push(estado_susp_ready, (void *)pcb);
    pthread_mutex_unlock(&mutex_estado_susp_ready);
} /*

 // void addEstadoExec(t_pcb * pcb){
 //     pthread_mutex_lock(&mutex_estado_exec);
 //     list_add(estado_exec, (void *) pcb);
 //     pthread_mutex_unlock(&mutex_estado_exec);
 // }
 // void addEstadoExit(t_pcb * pcb){
 //     pthread_mutex_lock(&mutex_estado_exit);
 //     list_add(estado_exit, (void *) pcb);
 //     pthread_mutex_unlock(&mutex_estado_exit);
 // }

 void * inicializarPcb(t_proceso * proceso){
     t_pcb * nuevo_pcb = malloc(sizeof(t_pcb));
     nuevo_pcb = crearPcb(proceso);
     list_add(estado_ready, nuevo_pcb);
     return nuevo_pcb;
 }
 */