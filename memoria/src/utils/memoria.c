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
    memoria = malloc(TAM_MEMORIA);
    uint32_t CANTIDAD_FRAMES = TAM_MEMORIA/TAM_PAGINA;
    char * bitarrayy = asignar_bytes(CANTIDAD_FRAMES);
    bitarray = bitarray_create(bitarrayy,1);

    tablasPrimerNivel = list_create();
    tablasSegundoNivel = list_create();
}
//ENTRADAS_POR_TABLA
uint32_t inicializarEstructurasProceso(uint32_t tamanioProceso){
    uint32_t paginasQueOcupa = (uint32_t) ceil((double)tamanioProceso / (double)TAM_PAGINA);
    //CANTIDAD DE ENTRADAS DE SEGUNDO NIVEL QUE NECESITA
    uint32_t paginasDeSegundoNivelCompletas = (uint32_t) floor((double)paginasQueOcupa / (double)ENTRADAS_POR_TABLA);
    uint32_t entradasUltimaPaginaSegundoNivel = paginasQueOcupa % ENTRADAS_POR_TABLA;

    t_list * tablaPrimerNivel = list_create();
    if(paginasDeSegundoNivelCompletas > 0) {
        for(int i=0; i<paginasDeSegundoNivelCompletas;i++){
            t_entradaPrimerNivel * entrada = crearEntradaPrimerNivel(ENTRADAS_POR_TABLA);
            list_add(tablaPrimerNivel, entrada);
        }
    }
    if(paginasQueOcupa % ENTRADAS_POR_TABLA){
        t_entradaPrimerNivel * entrada = crearEntradaPrimerNivel(entradasUltimaPaginaSegundoNivel);
        list_add(tablaPrimerNivel, entrada)  ;
    }
    uint32_t indexTablaPrimerNivel = list_add(tablasPrimerNivel, tablaPrimerNivel);
    
    log_info(logger, "index: %d",indexTablaPrimerNivel);
    return indexTablaPrimerNivel;
}

t_entradaSegundoNivel * crearEntradaSegundoNivel() {
    t_entradaSegundoNivel * entrada = malloc(sizeof(t_entradaSegundoNivel));
    entrada->marco = 0;
    entrada->modificado = false;
    entrada->presencia = false;
    entrada->uso = false;

    return entrada;

}

t_list * crearTablaSegundoNivel(int entradas) {
    t_list * tabla = list_create();
    for(int j=0; j<entradas;j++){
        t_entradaSegundoNivel * entrada = crearEntradaSegundoNivel();
        list_add(tabla, entrada);
    }

    return tabla;
}

t_entradaPrimerNivel * crearEntradaPrimerNivel(int entradasSegundoNivel) {
    t_list * tablaSegundoNivel = crearTablaSegundoNivel(entradasSegundoNivel);
    t_entradaPrimerNivel * entrada = malloc(sizeof(t_entradaPrimerNivel)); 
    entrada->tablaSegundoNivel = list_add(tablasSegundoNivel, tablaSegundoNivel);

    return entrada;
}