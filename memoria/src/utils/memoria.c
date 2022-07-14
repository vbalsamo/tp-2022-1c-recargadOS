#include <utils/memoria.h>

char *asignar_bytes(int cant_frames)
{
    char *buf;
    int bytes;
    if (cant_frames < 8)
        bytes = 1;
    else
    {
        double c = (double)cant_frames;
        bytes = (int)ceil(c / 8.0);
    }
    buf = malloc(bytes);
    memset(buf, 0, bytes);
    return buf;
}

void iniciarEstructurasMemoria(void)
{
    numeroMarco = 0;
    memoria = string_repeat('X', TAM_MEMORIA);
    // memoria =malloc(TAM_MEMORIA);
    char *bitarrayy = asignar_bytes(CANTIDAD_FRAMES);
    bitarray = bitarray_create(bitarrayy, CANTIDAD_FRAMES / 8);
    for (int i = 0; i < CANTIDAD_FRAMES; i++)
    {
        bitarray_clean_bit(bitarray, i);
    }

    tablasPrimerNivel = list_create();
    tablasSegundoNivel = list_create();
    estadosPCBS = dictionary_create();
}

t_estadoPCB *newEstadoPCB(uint32_t indexTablaPaginasPrimerNivel)
{
    t_estadoPCB *estado = malloc(sizeof(t_estadoPCB));
    estado->marcosOcupados = 0;
    estado->punteroClock = 0;
    estado->indexTablaPaginasPrimerNivel = indexTablaPaginasPrimerNivel;
    return estado;
}

void *leerMarco(uint32_t numeroMarco)
{
    void *marco = malloc(TAM_PAGINA);
    int desplazamiento = numeroMarco * TAM_PAGINA;
    memcpy(marco, memoria + desplazamiento, TAM_PAGINA);
    return marco;
}

void escribirMarco(uint32_t numeroMarco, void *paginaSwap)
{
    int desplazamiento = numeroMarco * TAM_PAGINA;
    memcpy(memoria + desplazamiento, paginaSwap, TAM_PAGINA);
}

uint32_t *leerDireccionFisica(uint32_t direccionFisica)
{
    uint32_t *dato = malloc(sizeof(uint32_t));
    memcpy(dato, memoria + direccionFisica, sizeof(uint32_t));
    log_info(logger, "dato leido: %d en direccion fisica:%d", *dato, direccionFisica);
    return dato;
}

void writeEnMemoria(uint32_t direccionFisica, uint32_t dato)
{
    memcpy(memoria + direccionFisica, &dato, sizeof(uint32_t));
    log_info(logger, "dato escrito: %d en direccion fisica:%d", dato, direccionFisica);
}

uint32_t marcosProceso(uint32_t tamanioProceso)
{
    return (uint32_t)ceil((double)tamanioProceso / (double)TAM_PAGINA);
}

uint32_t inicializarEstructurasProceso(t_pcb *pcb)
{
    ID_EN_SWAP = 0;
    uint32_t marcosQueOcupa = marcosProceso(pcb->tamanioProceso);
    // CANTIDAD DE ENTRADAS DE SEGUNDO NIVEL QUE NECESITA
    uint32_t paginasDeSegundoNivelCompletas = (uint32_t)floor((double)marcosQueOcupa / (double)ENTRADAS_POR_TABLA);
    uint32_t entradasUltimaPaginaSegundoNivel = marcosQueOcupa % ENTRADAS_POR_TABLA;

    t_list *tablaPrimerNivel = list_create();
    if (paginasDeSegundoNivelCompletas > 0)
    {
        for (int i = 0; i < paginasDeSegundoNivelCompletas; i++)
        {
            t_entradaPrimerNivel *entrada = crearEntradaPrimerNivel(ENTRADAS_POR_TABLA, pcb->id);
            list_add(tablaPrimerNivel, entrada);
            log_info(logger, "creada tabla de primer nivel");
        }
    }
    if (entradasUltimaPaginaSegundoNivel > 0)
    {
        t_entradaPrimerNivel *entrada = crearEntradaPrimerNivel(entradasUltimaPaginaSegundoNivel, pcb->id);
        list_add(tablaPrimerNivel, entrada);
        log_info(logger, "creada tabla de primer nivel incompleta");
    }
    uint32_t indexTablaPrimerNivel = list_add(tablasPrimerNivel, tablaPrimerNivel);

    log_info(logger, "creada la tabla de paginas de primer nivel de indice: %d, para el pcb->id>:%d", indexTablaPrimerNivel, pcb->id);
    t_estadoPCB *estado = newEstadoPCB(indexTablaPrimerNivel);

    // ver si esto anda y pasar a funcion
    char *_id = stringID(pcb->id);
    dictionary_put(estadosPCBS, _id, estado);
    free(_id);
    return indexTablaPrimerNivel;
}
char *stringID(uint32_t _id)
{

    char *id = string_new();
    string_append_with_format(&id, "%d", _id);
    return id;
}

t_entradaSegundoNivel *crearEntradaSegundoNivel(uint32_t id)
{
    t_entradaSegundoNivel *entrada = malloc(sizeof(t_entradaSegundoNivel));
    entrada->marco = TAM_MEMORIA;
    entrada->modificado = false;
    entrada->presencia = false;
    entrada->uso = false;
    entrada->paginaSwap = ID_EN_SWAP;
    entrada->id = id;
    log_info(logger, "id_en_swap: %d", ID_EN_SWAP);
    ID_EN_SWAP++;
    return entrada;
}

t_list *crearTablaSegundoNivel(int entradas, uint32_t id)
{
    t_list *tabla = list_create();
    for (int j = 0; j < entradas; j++)
    {
        t_entradaSegundoNivel *entrada = crearEntradaSegundoNivel(id);
        list_add(tabla, entrada);
    }
    log_info(logger, "creada tabla de 2do nivel");
    return tabla;
}

t_entradaPrimerNivel *crearEntradaPrimerNivel(int entradasSegundoNivel, uint32_t id)
{
    t_list *tablaSegundoNivel = crearTablaSegundoNivel(entradasSegundoNivel, id);
    t_entradaPrimerNivel *entrada = malloc(sizeof(t_entradaPrimerNivel));
    entrada->tablaSegundoNivel = list_add(tablasSegundoNivel, tablaSegundoNivel);
    entrada->id = id;

    return entrada;
}

uint32_t obtenerTablaSegundoNivel(uint32_t indexTablaPrimerNivel, uint32_t entradaPagina)
{
    t_list *tablaPrimerNivel = list_get(tablasPrimerNivel, indexTablaPrimerNivel);
    t_entradaPrimerNivel *entrada = list_get(tablaPrimerNivel, entradaPagina);
    return entrada->tablaSegundoNivel;
}

uint32_t obtenerMarco(uint32_t indexTablaSegundoNivel, uint32_t entradaPagina, uint32_t id, bool esWrite)
{
    t_list *tablaSegundoNivel = list_get(tablasSegundoNivel, indexTablaSegundoNivel);
    t_entradaSegundoNivel *entrada = list_get(tablaSegundoNivel, entradaPagina);
    char *_id = stringID(id);
    t_estadoPCB *estado = dictionary_get(estadosPCBS, _id);
    if (!entrada->presencia)
    {
        if (estado->marcosOcupados < MARCOS_POR_PROCESO)
        {
            int marco = -1;
            do
            {
                marco++;
            } while (bitarray_test_bit(bitarray, marco));

            bitarray_set_bit(bitarray, marco);
            entrada->marco = marco;
            estado->marcosOcupados++;

            void *contenidoPaginaSwap = leerPaginaSwap(entrada->paginaSwap, id);
            escribirMarco(marco, contenidoPaginaSwap);
            free(contenidoPaginaSwap);
        }
        else
        {
            t_entradaSegundoNivel *victima = reemplazar(estado, entrada);
            if (victima->modificado)
            {
                void *contenidoMarco = leerMarco(victima->marco);
                escribirMarcoSwap(contenidoMarco, victima->paginaSwap, id);
                free(contenidoMarco);
                victima->modificado = false;
                victima->presencia = false;
                char *_idVictima = stringID(victima->id);
                t_estadoPCB * estadoVictima = dictionary_get(estadosPCBS, _idVictima);
                estadoVictima->marcosOcupados--;
                free(_idVictima);
            }

            entrada->marco = victima->marco;
            void *contenidoPaginaSwap = leerPaginaSwap(entrada->paginaSwap, id);
            escribirMarco(entrada->marco, contenidoPaginaSwap);
            free(contenidoPaginaSwap);
        }
        entrada->presencia = true;
    }
    if (esWrite)
    {
        entrada->modificado = true;
    }
    entrada->uso = true;
    free(_id);
    return entrada->marco;
}

void eliminarEntradaSegundoNivel(void *entrada)
{
    if (((t_entradaSegundoNivel *)entrada)->presencia)
    {
        // log_info(logger, "bit:%d valor:%d",((t_entradaSegundoNivel *)entrada)->marco, bitarray_test_bit(bitarray, ((t_entradaSegundoNivel *)entrada)->marco));
        bitarray_clean_bit(bitarray, ((t_entradaSegundoNivel *)entrada)->marco);
        // log_info(logger, "bit:%d valor:%d",((t_entradaSegundoNivel *)entrada)->marco, bitarray_test_bit(bitarray, ((t_entradaSegundoNivel *)entrada)->marco));
    }
}

void eliminarEntradaPrimerNivel(void *entrada)
{
    log_info(logger, "eliminado tabla de segundo nivel index:%d", ((t_entradaPrimerNivel *)entrada)->tablaSegundoNivel);
    t_list *tablaSegundoNivel = list_get(tablasSegundoNivel, ((t_entradaPrimerNivel *)entrada)->tablaSegundoNivel);
    list_iterate(tablaSegundoNivel, eliminarEntradaSegundoNivel);
}

void eliminarMarcos(int indexTablaPrimerNivel)
{
    log_info(logger, "eliminando tablas de primer nivel index: %d", indexTablaPrimerNivel);
    t_list *tablaPrimerNivel = list_get(tablasPrimerNivel, indexTablaPrimerNivel);
    list_iterate(tablaPrimerNivel, eliminarEntradaPrimerNivel);
}

void swapearEntradaSegundoNivel(void *entrada)
{
    t_entradaSegundoNivel *entradaSegundoNivel = (t_entradaSegundoNivel *)entrada;
    if (entradaSegundoNivel->presencia && entradaSegundoNivel->modificado)
    {
        void *marco = leerMarco(entradaSegundoNivel->marco);
        log_info(logger, "se lee el marco:%d", entradaSegundoNivel->marco);
        escribirMarcoSwap(marco, entradaSegundoNivel->paginaSwap, entradaSegundoNivel->id);
        bitarray_clean_bit(bitarray, entradaSegundoNivel->marco);
        free(marco);
    }
}

void swapearEntradaPrimerNivel(void *entrada)
{
    t_entradaPrimerNivel *entradaPrimerNivel = (t_entradaPrimerNivel *)entrada;
    log_info(logger, "swapeando tabla de segundo nivel index:%d", entradaPrimerNivel->tablaSegundoNivel);
    t_list *tablaSegundoNivel = list_get(tablasSegundoNivel, entradaPrimerNivel->tablaSegundoNivel);
    list_iterate(tablaSegundoNivel, swapearEntradaSegundoNivel);
}

void suspenderProceso(t_pcb *pcb)
{
    t_list *tablaPrimerNivel = list_get(tablasPrimerNivel, pcb->tablaDePaginas);
    log_info(logger, "swapeando tabla de primerNivel nivel index:%d", pcb->tablaDePaginas);
    list_iterate(tablaPrimerNivel, swapearEntradaPrimerNivel);
    char *_id = stringID(pcb->id);
    t_estadoPCB * estado = dictionary_get(estadosPCBS, _id);
    estado->marcosOcupados=0;
    free(_id);
}

t_entradaSegundoNivel *reemplazar(t_estadoPCB *estado, t_entradaSegundoNivel *entrada)
{
    t_entradaSegundoNivel *entradaRemplazar = NULL;
    t_list *entradasSegundoNivel = obtenerEntradasSegundoNivel(estado->indexTablaPaginasPrimerNivel);
    if (string_equals_ignore_case(ALGORITMO_REEMPLAZO, "CLOCK"))
    {
        entradaRemplazar = reemplazarClock(estado, entradasSegundoNivel, entrada);
    }
    else if (string_equals_ignore_case(ALGORITMO_REEMPLAZO, "CLOCK-M"))
    {
        entradaRemplazar = reemplazarClockM(estado, entradasSegundoNivel, entrada);
    }
    else
    {
        log_error(logger, "ALGORITMO_REEMPLAZO: %s no contemplado", ALGORITMO_REEMPLAZO);
        exit(-1);
    }
    list_destroy(entradasSegundoNivel);
    return entradaRemplazar;
}

t_list *obtenerEntradasSegundoNivel(uint32_t indexTablaPaginasPrimerNivel)
{
    t_list *tablaPrimerNivel = list_get(tablasPrimerNivel, indexTablaPaginasPrimerNivel);
    t_list *entradasSegundoNivel = list_create();

    void iterarPrimerNivel(t_entradaPrimerNivel * entrada)
    {
        t_list *tablaSegundoNivel = list_get(tablasSegundoNivel, entrada->tablaSegundoNivel);

        void iterarSegundoNivel(t_entradaSegundoNivel * entrada)
        {
            list_add(entradasSegundoNivel, entrada);
        }
        list_iterate(tablaSegundoNivel, (void *)iterarSegundoNivel);
    }
    list_iterate(tablaPrimerNivel, (void *)iterarPrimerNivel);
    return entradasSegundoNivel;
}

t_entradaSegundoNivel *reemplazarClock(t_estadoPCB *estado, t_list *entradasSegundoNivel, t_entradaSegundoNivel *entrada)
{
    int cant_entradas = list_size(entradasSegundoNivel);
    t_entradaSegundoNivel *entradaRemplazar;
    void imprimirBitsUso(t_entradaSegundoNivel * entrada)
    {
        if (entrada->presencia)
            log_info(logger, "PAGINASWAP: %d - FRAME: %d - BIT USO: %d - BIT PRESENCIA: %d", entrada->paginaSwap, entrada->marco, entrada->uso, entrada->presencia);
    }

    list_iterate(entradasSegundoNivel, (void *)imprimirBitsUso);

    t_entradaSegundoNivel *recorredorEntrada;

    while (1)
    {

        recorredorEntrada = list_get(entradasSegundoNivel, estado->punteroClock);
        estado->punteroClock = (estado->punteroClock + 1) % cant_entradas;

        if (recorredorEntrada->presencia)
        {
            if (!recorredorEntrada->uso)
            {
                entradaRemplazar = recorredorEntrada;
                log_info(logger, "Victima Algoritmo %s: paginaSwap Reemplazada:%d - paginaSwap Nueva:%d  - frame:%d", ALGORITMO_REEMPLAZO,
                         entradaRemplazar->paginaSwap, entrada->paginaSwap, entradaRemplazar->marco);
                break;
            }
            else
            {
                recorredorEntrada->uso = false;
            }
        }
    }
    return entradaRemplazar;
}

t_entradaSegundoNivel *reemplazarClockM(t_estadoPCB *estado, t_list *entradasSegundoNivel, t_entradaSegundoNivel *entrada)
{
    int cant_entradas = list_size(entradasSegundoNivel);
    t_entradaSegundoNivel *entradaRemplazar;
    void imprimirBitsUso(t_entradaSegundoNivel * entrada)
    {
        if (entrada->presencia)
            log_info(logger, "PAGINASWAP: %d - FRAME: %d - BIT USO: %d - BIT PRESENCIA: %d - BIT MODIFICADO: %d", entrada->paginaSwap, entrada->marco, entrada->uso, entrada->presencia, entrada->modificado);
    }

    list_iterate(entradasSegundoNivel, (void *)imprimirBitsUso);

    t_entradaSegundoNivel *recorredorEntrada;

    while (1)
    {

        recorredorEntrada = list_get(entradasSegundoNivel, estado->punteroClock);
        estado->punteroClock = (estado->punteroClock + 1) % cant_entradas;

        if (recorredorEntrada->presencia)
        {

            if (!(recorredorEntrada->modificado || recorredorEntrada->uso))
            {
                entradaRemplazar = recorredorEntrada;
                log_info(logger, "Victima Algoritmo %s: paginaSwapReemplazada:%d -paginaSwapNueva: %d - frame:%d", ALGORITMO_REEMPLAZO,
                         entradaRemplazar->paginaSwap, entrada->paginaSwap, entradaRemplazar->marco);
                break;
            }
            else if (!recorredorEntrada->uso)
            {
                entradaRemplazar = recorredorEntrada;
                log_info(logger, "Victima Algoritmo %s: paginaSwapReemplazada:%d -paginaSwapNueva: %d - frame:%d", ALGORITMO_REEMPLAZO,
                         entradaRemplazar->paginaSwap, entrada->paginaSwap, entradaRemplazar->marco);
                break;
            }
            else
            {
                recorredorEntrada->uso = false;
            }
        }
    }
    return entradaRemplazar;
}

void retardoMemoria()
{
    log_info(logger, "esperando retardo memoria de %d milisegundos", RETARDO_MEMORIA);
    sleep(RETARDO_MEMORIA);
}