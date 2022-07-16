#include <utils/mmu.h>
void iniciarEstructurasMMU() {
    tiempoAccesoGlobal = 0;
    indiceFIFO = 0;
    listaTLB = list_create();
    PCB_ACTUAL = 0;
}

t_EntradaTLB * newEntradaTLB(uint32_t pagina, uint32_t marco) {
    t_EntradaTLB * nuevaEntrada = malloc(sizeof(t_EntradaTLB));
    nuevaEntrada->pagina = pagina;
    nuevaEntrada->marco = marco;
    nuevaEntrada->tiempoAcceso = tiempoAccesoGlobal;
    tiempoAccesoGlobal++;
    return nuevaEntrada;
}

//falta agregar a tlb cuando hay espacio vacio
void agregarTLB(uint32_t pagina, uint32_t marco) {
    if(list_size(listaTLB)<ENTRADAS_TLB){
        t_EntradaTLB * entrada = newEntradaTLB(pagina, marco);
        list_add(listaTLB, entrada);
    }
    else {
        if(string_equals_ignore_case(REEMPLAZO_TLB, "FIFO")){
            reemplazarTLB_FIFO(pagina, marco);
        }
        else if(string_equals_ignore_case(REEMPLAZO_TLB, "LRU")){
            reemplazarTLB_LRU(pagina, marco);
        }
        else {
            log_error(logger, "REMPLAZO_TLB: %s no contemplado", REEMPLAZO_TLB);
            exit(-1);
        }
    }
    //imprimirEntradasTLB();
}

void imprimirEntradasTLB(){
    list_iterate(listaTLB, (void*) imprimirEntradaTLB);
}
void imprimirEntradaTLB(t_EntradaTLB* entrada){
    log_info(logger, "ENTRADA_TLB: página: %d - marco: %d - tiempoAcceso: %d", entrada->pagina, entrada->marco, entrada->tiempoAcceso);
}

// void imprimirBitsUso(t_entradaSegundoNivel* entrada) {
//         if(entrada->presencia)
//             log_info(logger, "PAGINASWAP: %d - FRAME: %d - BIT USO: %d - BIT PRESENCIA: %d", entrada->paginaSwap, entrada->marco, entrada->uso, entrada->presencia);
//     }

//     list_iterate(entradasSegundoNivel, (void*) imprimirBitsUso);

void vaciarTLB(uint32_t pcb_nuevo){
    void destroyer(void * entrada){
        free(entrada);
    }
    if(PCB_ACTUAL != pcb_nuevo){
        list_clean_and_destroy_elements(listaTLB, destroyer);
        log_info(logger, "TLB vaciada por cambio de proceso");
    }
    else{
        log_info(logger, "No se vacía la TLB porque sigue ejecutando el mismo proceso");
    }
    PCB_ACTUAL = pcb_nuevo;
}

void actualizarTLB(t_EntradaTLB * entrada) {
    entrada->tiempoAcceso = tiempoAccesoGlobal;
    tiempoAccesoGlobal++;
}

void reemplazarTLB_FIFO(uint32_t pagina, uint32_t marco) {
    t_EntradaTLB * nuevaEntrada = newEntradaTLB(pagina, marco);
    t_EntradaTLB * viejaEntrada = list_replace(listaTLB, indiceFIFO, nuevaEntrada);
    //log_info(logger, "pagina viejaEntrada:%d - marco viejaEntrada:%d - pagina nuevaEntrada: %d - marco nuevaEntrada:%d", viejaEntrada->pagina, viejaEntrada->marco, nuevaEntrada->pagina, nuevaEntrada->marco);
    free(viejaEntrada);
    incrementarIndiceFIFO();
}

void reemplazarTLB_LRU(uint32_t pagina, uint32_t marco) {
    //last recently used
    t_EntradaTLB * nuevaEntrada = newEntradaTLB(pagina, marco);
    bool comparator(void * tlb1, void * tlb2) {
        return ((t_EntradaTLB *) tlb1)->tiempoAcceso < ((t_EntradaTLB *) tlb2)->tiempoAcceso;
    }
    list_sort(listaTLB, comparator);
    t_EntradaTLB * viejaEntrada = list_remove(listaTLB,0);
    list_add(listaTLB, nuevaEntrada);
    //log_info(logger, "pagina viejaEntrada:%d - marco viejaEntrada:%d - pagina nuevaEntrada: %d - marco nuevaEntrada:%d", viejaEntrada->pagina, viejaEntrada->marco, nuevaEntrada->pagina, nuevaEntrada->marco);
    free(viejaEntrada);
}


t_EntradaTLB * obtenerEntradaTLB(uint32_t pagina) {
    bool condicion(void * tlb) {
        return ((t_EntradaTLB *) tlb)->pagina == pagina;
    }
    t_EntradaTLB * entrada = list_find(listaTLB, condicion);
    return entrada;
}


void incrementarIndiceFIFO() {
    indiceFIFO = (indiceFIFO + 1) % ENTRADAS_TLB;
}

uint32_t consultarTablaSegundoNivel(uint32_t tablaDePaginasPrimerNivel, uint32_t pagina) {
    int socket_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA);
    uint32_t entradaPrimerNivel = obtenerEntradaTabla1erNivel(pagina);
    t_consultaTablaPagina * consulta = malloc(sizeof(t_consultaTablaPagina));
    
    consulta->tablaDePaginas = tablaDePaginasPrimerNivel;
    consulta->entradaPagina = entradaPrimerNivel;
    consulta->id = PCB_ACTUAL;//TODO: problema de concurrencia
    
    t_paquete * paquete = armarPaqueteCon(consulta, REQ_TABLA_SEGUNDO_NIVEL_CPU_MEMORIA);
    enviarPaquete(paquete,socket_memoria);
    free(consulta);
    eliminarPaquete(paquete);
    t_paquete * paqueteRespuesta = recibirPaquete(socket_memoria);
    uint32_t * tablaSegundoNivelDeserializada = deserializarUINT32_T(paqueteRespuesta->buffer->stream);
    eliminarPaquete(paqueteRespuesta);
    uint32_t tablaSegundoNivel = *tablaSegundoNivelDeserializada; 
    free(tablaSegundoNivelDeserializada);
    return tablaSegundoNivel;
}

uint32_t consultarMarco(uint32_t tablaDePaginasSegundoNivel, uint32_t pagina, t_cod_op codOP) {
    uint32_t socket_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA);
    uint32_t entradaSegundoNivel = obtenerEntradaTabla2doNivel(pagina);
    t_consultaTablaPagina * consulta = malloc(sizeof(t_consultaTablaPagina));
    
    consulta->tablaDePaginas = tablaDePaginasSegundoNivel;
    consulta->entradaPagina = entradaSegundoNivel;
    consulta->id = PCB_ACTUAL;//TODO: problema de concurrencia

    t_paquete * paquete = armarPaqueteCon(consulta, codOP);
    enviarPaquete(paquete,socket_memoria);
    free(consulta);
    eliminarPaquete(paquete);
    t_paquete * paqueteRespuesta = recibirPaquete(socket_memoria);
    
    uint32_t * marco = deserializarUINT32_T(paqueteRespuesta->buffer->stream);
    eliminarPaquete(paqueteRespuesta);
    uint32_t marcoo = * marco;
    free(marco);
    return marcoo;
}

uint32_t consultarDireccionFisica(uint32_t tablaPaginasPrimerNivelPCB, uint32_t direccion_logica, t_cod_op codOP) {
    uint32_t pagina = obtenerNumeroPagina(direccion_logica);
    t_EntradaTLB * entrada = obtenerEntradaTLB(pagina);
    uint32_t marco;
    
    if(entrada==NULL) { //Si no esta en TLB
        uint32_t tablaDePaginasSegundoNivel = consultarTablaSegundoNivel(tablaPaginasPrimerNivelPCB, pagina);
        marco = consultarMarco(tablaDePaginasSegundoNivel, pagina, codOP);
        log_info(logger, "TLB MISS: página:%d - marco:%d NO encontrada en TLB, buscando en memoria", pagina, marco);
        agregarTLB(pagina, marco);
    }
    else {
        marco = entrada->marco;
        log_info(logger, "TLB HIT: página:%d - marco:%d encontrada en TLB", pagina, marco);
        actualizarTLB(entrada);
    }

    uint32_t desplazamiento = obtenerDesplazamiento(direccion_logica, pagina);
    uint32_t direccionFisica = obtenerDireccionFisica(desplazamiento, marco);
    return direccionFisica;
}

uint32_t obtenerNumeroPagina(uint32_t direccion_logica) {
    return floor(direccion_logica/traduccion_direcciones->tamanio_pagina);
}

uint32_t obtenerEntradaTabla1erNivel(uint32_t numero_pagina) {
    return floor(numero_pagina/traduccion_direcciones->paginas_por_tabla);
}

uint32_t obtenerEntradaTabla2doNivel(uint32_t numero_pagina) {
    return numero_pagina % traduccion_direcciones->paginas_por_tabla;
}

uint32_t obtenerDesplazamiento(uint32_t direccion_logica, uint32_t numero_pagina) {
    return direccion_logica - numero_pagina * traduccion_direcciones->tamanio_pagina;
}

uint32_t obtenerDireccionFisica(uint32_t desplazamiento, uint32_t numero_marco) {
    return desplazamiento + numero_marco * traduccion_direcciones->tamanio_pagina;
}