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
void inicializarEstructurasProceso(uint32_t tamanioProceso){
    uint32_t paginasQueOcupa = (uint32_t) ceil((double)tamanioProceso / (double)TAM_PAGINA);
    //CANTIDAD DE ENTRADAS DE SEGUNDO NIVEL QUE NECESITA
    
    uint32_t paginasDeSegundoNivelCompletas = (uint32_t) floor((double)paginasQueOcupa / (double)ENTRADAS_POR_TABLA);
    //
    uint32_t entradasUltimaPaginaSegundoNivel = paginasQueOcupa % ENTRADAS_POR_TABLA;
    t_tablaPrimerNivel * tablaPrimerNivel = malloc(sizeof(t_tablaPrimerNivel));
    tablaPrimerNivel->tabla = list_create();
    if(paginasDeSegundoNivelCompletas > 0) {
        for(int i=0; i<paginasDeSegundoNivelCompletas;i++){
            t_tablaSegundoNivel * tablaSegundoNivel = malloc(sizeof(t_tablaSegundoNivel ));
            tablaSegundoNivel->tabla = list_create();

            for(int j=0; j<ENTRADAS_POR_TABLA;j++){
                t_entradaSegundoNivel * entrada = malloc(sizeof(t_entradaSegundoNivel));
                list_add(tablaSegundoNivel->tabla, entrada);
            }
            
            int indexTablaSegundoNivel = list_add(tablasSegundoNivel, tablaSegundoNivel);
            t_entradaPrimerNivel * entrada = malloc(sizeof(t_entradaPrimerNivel)); 
            entrada->tablaSegundoNivel = indexTablaSegundoNivel;
            
            list_add(tablaPrimerNivel->tabla, entrada);
        }
    }
    int indexTablaPrimerNivel;
    if((paginasQueOcupa % ENTRADAS_POR_TABLA)>0){
        t_tablaSegundoNivel * tablaSegundoNivel = malloc(sizeof(t_tablaSegundoNivel ));
        tablaSegundoNivel->tabla = list_create();

        for(int j=0; j<entradasUltimaPaginaSegundoNivel;j++){
            t_entradaSegundoNivel * entrada = malloc(sizeof(t_entradaSegundoNivel));
            list_add(tablaSegundoNivel->tabla, entrada);
        }
        
        int indexTablaSegundoNivel = list_add(tablasSegundoNivel, tablaSegundoNivel);
        t_entradaPrimerNivel * entrada = malloc(sizeof(t_entradaPrimerNivel)); 
        entrada->tablaSegundoNivel = indexTablaSegundoNivel;
        
        list_add(tablaPrimerNivel->tabla, entrada);
            
    }
    indexTablaPrimerNivel = list_add(tablasPrimerNivel, tablaPrimerNivel);
    
    log_info(logger, "index: %d",indexTablaPrimerNivel);
    

}