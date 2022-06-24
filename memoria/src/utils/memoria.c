#include <utils/memoria.h>

char* asignar_bytes(int cant_frames) {
    char* buf;
    int bytes;
    if(cant_frames < 8)
        bytes = 1;
    else
    {
        double c = (double) cant_frames;
        bytes = (int) ceil(c/8.0);
    }
    buf = malloc(bytes);
    memset(buf,0,bytes);
    return buf;
}

void iniciarEstructurasMemoria(void) {
    numeroMarco=0;
    memoria =string_repeat('X', TAM_MEMORIA);
    //memoria =malloc(TAM_MEMORIA);
    char * bitarrayy = asignar_bytes(CANTIDAD_FRAMES);
    bitarray = bitarray_create(bitarrayy,CANTIDAD_FRAMES/8);
    for(int i=0; i<CANTIDAD_FRAMES; i++) {
        bitarray_set_bit(bitarray,i);
        //bool test = bitarray_test_bit(bitarray,i);
    }


    tablasPrimerNivel = list_create();
    tablasSegundoNivel = list_create();
}

void * leerMarco(uint32_t numeroMarco) {
    void * marco = malloc(TAM_PAGINA);
    int desplazamiento = numeroMarco * TAM_PAGINA;
    memcpy(marco, memoria + desplazamiento, TAM_PAGINA);
    return marco;
}

uint32_t * leerDireccionFisica(uint32_t direccionFisica) {
    uint32_t * dato = malloc(sizeof(uint32_t));
    memcpy(dato, memoria + direccionFisica, sizeof(uint32_t));
    log_info(logger, "dato leido: %d en direccion fisica:%d", *dato, direccionFisica);
    return dato;
}

void writeEnMemoria(uint32_t direccionFisica, uint32_t dato){
    memcpy(memoria + direccionFisica, dato, sizeof(uint32_t));
    log_info(logger, "dato escrito: %d en direccion fisica:%d", *dato, direccionFisica);
}

uint32_t marcosProceso(uint32_t tamanioProceso) {
    return (uint32_t) ceil((double)tamanioProceso / (double)TAM_PAGINA);
}

uint32_t inicializarEstructurasProceso(uint32_t tamanioProceso){
    ID_EN_SWAP = 0;
    uint32_t marcosQueOcupa = marcosProceso(tamanioProceso);
    //CANTIDAD DE ENTRADAS DE SEGUNDO NIVEL QUE NECESITA
    uint32_t paginasDeSegundoNivelCompletas = (uint32_t) floor((double)marcosQueOcupa / (double)ENTRADAS_POR_TABLA);
    uint32_t entradasUltimaPaginaSegundoNivel = marcosQueOcupa % ENTRADAS_POR_TABLA;

    t_list * tablaPrimerNivel = list_create();
    if(paginasDeSegundoNivelCompletas > 0) {
        for(int i=0; i<paginasDeSegundoNivelCompletas;i++){
            t_entradaPrimerNivel * entrada = crearEntradaPrimerNivel(ENTRADAS_POR_TABLA);
            list_add(tablaPrimerNivel, entrada);
            log_info(logger, "creada tabla de primer nivel");
        }
    }
    if(entradasUltimaPaginaSegundoNivel>0){
        t_entradaPrimerNivel * entrada = crearEntradaPrimerNivel(entradasUltimaPaginaSegundoNivel);
        list_add(tablaPrimerNivel, entrada);
        log_info(logger, "creada tabla de primer nivel incompleta");
    }
    uint32_t indexTablaPrimerNivel = list_add(tablasPrimerNivel, tablaPrimerNivel);
    
    log_info(logger, "index: %d",indexTablaPrimerNivel);

    return indexTablaPrimerNivel;
}

t_entradaSegundoNivel * crearEntradaSegundoNivel() {
    t_entradaSegundoNivel * entrada = malloc(sizeof(t_entradaSegundoNivel));
    bool random = numeroMarco%2;
    entrada->marco = numeroMarco++;
    //entrada->marco = 0;
    entrada->modificado = random;
    //entrada->presencia = false;
    entrada->presencia = random;
    entrada->uso = false;
    entrada->paginaSwap = ID_EN_SWAP;
    log_info(logger, "id_en_swap: %d", ID_EN_SWAP);
    ID_EN_SWAP++;
    return entrada;
}

t_list * crearTablaSegundoNivel(int entradas) {
    t_list * tabla = list_create();
    for(int j=0; j<entradas;j++){
        t_entradaSegundoNivel * entrada = crearEntradaSegundoNivel();
        list_add(tabla, entrada);
    }
    log_info(logger, "creada tabla de 2do nivel");
    return tabla;
}

t_entradaPrimerNivel * crearEntradaPrimerNivel(int entradasSegundoNivel) {
    t_list * tablaSegundoNivel = crearTablaSegundoNivel(entradasSegundoNivel);
    t_entradaPrimerNivel * entrada = malloc(sizeof(t_entradaPrimerNivel)); 
    entrada->tablaSegundoNivel = list_add(tablasSegundoNivel, tablaSegundoNivel);

    return entrada;
}

uint32_t obtenerTablaSegundoNivel(uint32_t indexTablaPrimerNivel, uint32_t entradaPagina) {
    t_list * tablaPrimerNivel = list_get(tablasPrimerNivel, indexTablaPrimerNivel);
    t_entradaPrimerNivel * entrada = list_get(tablaPrimerNivel, entradaPagina);
    return entrada->tablaSegundoNivel; 
}
//  typedef struct{
//         uint32_t marco;
//         bool presencia;
//         bool uso;
//         bool modificado;
//     }t_entradaSegundoNivel;
uint32_t obtenerMarco(uint32_t indexTablaSegundoNivel, uint32_t entradaPagina) {
    t_list * tablaSegundoNivel = list_get(tablasSegundoNivel, indexTablaSegundoNivel);
    t_entradaSegundoNivel * entrada = list_get(tablaSegundoNivel, entradaPagina);
    
    //hacer todo el tema de algoritmos de desalojo
    return entrada->marco;
}

void eliminarEntradaSegundoNivel(void * entrada) { 
    if (((t_entradaSegundoNivel *)entrada)->presencia) {
        //log_info(logger, "bit:%d valor:%d",((t_entradaSegundoNivel *)entrada)->marco, bitarray_test_bit(bitarray, ((t_entradaSegundoNivel *)entrada)->marco));
        bitarray_clean_bit(bitarray, ((t_entradaSegundoNivel *)entrada)->marco);
        //log_info(logger, "bit:%d valor:%d",((t_entradaSegundoNivel *)entrada)->marco, bitarray_test_bit(bitarray, ((t_entradaSegundoNivel *)entrada)->marco));
    }
}

void eliminarEntradaPrimerNivel(void * entrada) {
    log_info(logger, "eliminado tabla de segundo nivel index:%d", ((t_entradaPrimerNivel*)entrada)->tablaSegundoNivel);
    t_list * tablaSegundoNivel = list_get(tablasSegundoNivel, ((t_entradaPrimerNivel*)entrada)->tablaSegundoNivel);
    list_iterate(tablaSegundoNivel, eliminarEntradaSegundoNivel);
}

void eliminarMarcos(int indexTablaPrimerNivel) {
    log_info(logger, "eliminando tablas de primer nivel index: %d",indexTablaPrimerNivel);
    t_list * tablaPrimerNivel = list_get(tablasPrimerNivel, indexTablaPrimerNivel);
    list_iterate(tablaPrimerNivel, eliminarEntradaPrimerNivel);
}

void swapearEntradaSegundoNivel(void * entrada) {
    t_entradaSegundoNivel* entradaSegundoNivel = (t_entradaSegundoNivel*) entrada;
    if (entradaSegundoNivel->presencia && entradaSegundoNivel->modificado) {
        void * marco = leerMarco(entradaSegundoNivel->marco);
        //chequear que el PCB_ID glonal en memoria funcione bien
        escribirMarcoSwap(marco, entradaSegundoNivel->paginaSwap, PCB_ID);
        bitarray_clean_bit(bitarray, entradaSegundoNivel->marco);
    }
}

void swapearEntradaPrimerNivel(void * entrada) {
    t_entradaPrimerNivel* entradaPrimerNivel = (t_entradaPrimerNivel*) entrada;
    log_info(logger, "swapeando tabla de segundo nivel index:%d", entradaPrimerNivel->tablaSegundoNivel);
    t_list * tablaSegundoNivel = list_get(tablasSegundoNivel, entradaPrimerNivel->tablaSegundoNivel);
    list_iterate(tablaSegundoNivel, swapearEntradaSegundoNivel);
}

void suspenderProceso(t_pcb * pcb){
    PCB_ID = pcb->id;
    t_list * tablaPrimerNivel = list_get(tablasPrimerNivel, pcb->tablaDePaginas);
    log_info(logger, "swapeando tabla de primerNivel nivel index:%d", pcb->tablaDePaginas);
    list_iterate(tablaPrimerNivel, swapearEntradaPrimerNivel);
    //desplazamiento = tam pag * num pag
    //fwrite(tabladepaginas)
}