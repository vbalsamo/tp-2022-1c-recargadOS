#include <utils/mmu.h>
void iniciarEstructurasMMU() {
    tiempoAccesoGlobal = 0;
    indiceFIFO = 0;
    listaTLB = list_create();
}

t_TLB * new_TLB(uint32_t pagina, uint32_t marco, uint32_t tiempoAcceso) {
    t_TLB * nuevaEntrada = malloc(sizeof(t_TLB));
    nuevaEntrada->pagina = pagina;
    nuevaEntrada->marco = marco;
    nuevaEntrada->tiempoAcceso = tiempoAcceso;
    return nuevaEntrada;
}

//falta agregar a tlb cuando hay espacio vacio

void reemplazarTLB_FIFO(uint32_t pagina, uint32_t marco) {
    t_TLB * nuevaEntrada = new_TLB(pagina, marco,0);
    t_TLB * viejaEntrada = list_replace(listaTLB, indiceFIFO, nuevaEntrada);
    free(viejaEntrada);
    incrementarIndiceFIFO();
}

void reemplazarTLB_LRU(uint32_t pagina, uint32_t marco) {
    //last recently used
    t_TLB * nuevaEntrada = new_TLB(pagina, marco,tiempoAccesoGlobal);
    tiempoAccesoGlobal++;
    bool comparator(void * tlb1, void * tlb2) {
        return ((t_TLB *) tlb1)->tiempoAcceso < ((t_TLB *) tlb2)->tiempoAcceso;
    }
    list_sort(listaTLB, comparator);
    t_TLB * viejaEntrada = list_remove(listaTLB,0);
    list_add(listaTLB, nuevaEntrada);
    free(viejaEntrada);
}


t_TLB * obtenerEntradaTLB(uint32_t pagina) {
    bool condicion(void * tlb) {
        return ((t_TLB *) tlb)->pagina == pagina;
    }
    t_TLB * entrada = list_find(listaTLB, condicion);
    return entrada;
}

void incrementarIndiceFIFO() {
    indiceFIFO = (indiceFIFO + 1) % ENTRADAS_TLB;
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