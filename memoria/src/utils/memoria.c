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
    marco=0;
    memoria = malloc(TAM_MEMORIA);
    uint32_t CANTIDAD_FRAMES = TAM_MEMORIA/TAM_PAGINA;
    char * bitarrayy = asignar_bytes(CANTIDAD_FRAMES);
    bitarray = bitarray_create(bitarrayy,CANTIDAD_FRAMES/8);
    for(int i=0; i<CANTIDAD_FRAMES; i++) {
        bitarray_set_bit(bitarray,i);
        //bool test = bitarray_test_bit(bitarray,i);
    }


    tablasPrimerNivel = list_create();
    tablasSegundoNivel = list_create();
}

uint32_t inicializarEstructurasProceso(uint32_t tamanioProceso){
    uint32_t marcosQueOcupa = (uint32_t) ceil((double)tamanioProceso / (double)TAM_PAGINA);
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
    entrada->marco = marco++;
    //entrada->marco = 0;
    entrada->modificado = false;
    //entrada->presencia = false;
    entrada->presencia = true;
    entrada->uso = false;

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
//  typedef struct{
//         uint32_t marco;
//         bool presencia;
//         bool uso;
//         bool modificado;
//     }t_entradaSegundoNivel;

void eliminarEntradaSegundoNivel(void * entrada) { 
    if (((t_entradaSegundoNivel *)entrada)->presencia) {
        log_info(logger, "bit:%d valor:%d",((t_entradaSegundoNivel *)entrada)->marco, bitarray_test_bit(bitarray, ((t_entradaSegundoNivel *)entrada)->marco));
        bitarray_clean_bit(bitarray, ((t_entradaSegundoNivel *)entrada)->marco);
        log_info(logger, "bit:%d valor:%d",((t_entradaSegundoNivel *)entrada)->marco, bitarray_test_bit(bitarray, ((t_entradaSegundoNivel *)entrada)->marco));
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