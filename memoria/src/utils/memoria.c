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
        
    pthread_mutex_init(&mutex_tablas_primer_nivel, (void *)NULL);
    pthread_mutex_init(&mutex_tablas_segundo_nivel, (void *)NULL);
    pthread_mutex_init(&mutex_estados_pcb, (void *)NULL);
    pthread_mutex_init(&mutex_id_en_swap, (void *)NULL);
    pthread_mutex_init(&mutex_memoria, (void*)NULL);
    pthread_mutex_init(&mutex_bitarray, (void*)NULL);
    pthread_mutex_init(&mutex_entrada_segundo_nivel, (void*)NULL);
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
    pthread_mutex_lock(&mutex_memoria);
    memcpy(marco, memoria + desplazamiento, TAM_PAGINA);
    pthread_mutex_unlock(&mutex_memoria);
    return marco;
}

void escribirMarco(uint32_t numeroMarco, void *paginaSwap)
{
    int desplazamiento = numeroMarco * TAM_PAGINA;
    pthread_mutex_lock(&mutex_memoria);
    memcpy(memoria + desplazamiento, paginaSwap, TAM_PAGINA);
    pthread_mutex_unlock(&mutex_memoria);
}

uint32_t *leerDireccionFisica(uint32_t direccionFisica)
{
    uint32_t *dato = malloc(sizeof(uint32_t));
    pthread_mutex_lock(&mutex_memoria);
    memcpy(dato, memoria + direccionFisica, sizeof(uint32_t));
    pthread_mutex_unlock(&mutex_memoria);
    log_info(logger, "dato leido: %d en direccion fisica:%d", *dato, direccionFisica);
    return dato;
}

void writeEnMemoria(uint32_t direccionFisica, uint32_t dato)
{   
    pthread_mutex_lock(&mutex_memoria);
    memcpy(memoria + direccionFisica, &dato, sizeof(uint32_t));
    pthread_mutex_unlock(&mutex_memoria);
    log_info(logger, "dato escrito: %d en direccion fisica:%d", dato, direccionFisica);
}

uint32_t marcosProceso(uint32_t tamanioProceso)
{
    return (uint32_t)ceil((double)tamanioProceso / (double)TAM_PAGINA);
}

uint32_t inicializarEstructurasProceso(t_pcb *pcb)
{
    uint32_t ID_EN_SWAP = 0;
    uint32_t marcosQueOcupa = marcosProceso(pcb->tamanioProceso);
    // CANTIDAD DE ENTRADAS DE SEGUNDO NIVEL QUE NECESITA
    uint32_t paginasDeSegundoNivelCompletas = (uint32_t)floor((double)marcosQueOcupa / (double)ENTRADAS_POR_TABLA);
    uint32_t entradasUltimaPaginaSegundoNivel = marcosQueOcupa % ENTRADAS_POR_TABLA;

    t_list *tablaPrimerNivel = list_create();
    if (paginasDeSegundoNivelCompletas > 0)
    {
        for (int i = 0; i < paginasDeSegundoNivelCompletas; i++)
        {
            t_entradaPrimerNivel *entrada = crearEntradaPrimerNivel(ENTRADAS_POR_TABLA, pcb->id, &ID_EN_SWAP);
            list_add(tablaPrimerNivel, entrada);
            log_info(logger, "creada tabla de primer nivel");
        }
    }
    if (entradasUltimaPaginaSegundoNivel > 0)
    {
        t_entradaPrimerNivel *entrada = crearEntradaPrimerNivel(entradasUltimaPaginaSegundoNivel, pcb->id, &ID_EN_SWAP);
        list_add(tablaPrimerNivel, entrada);
        log_info(logger, "creada tabla de primer nivel incompleta");
    }
    pthread_mutex_lock(&mutex_tablas_primer_nivel);
    uint32_t indexTablaPrimerNivel = list_add(tablasPrimerNivel, tablaPrimerNivel);
    pthread_mutex_unlock(&mutex_tablas_primer_nivel);

    log_info(logger, "creada la tabla de paginas de primer nivel de indice: %d, para el pcb->id>:%d", indexTablaPrimerNivel, pcb->id);
    t_estadoPCB *estado = newEstadoPCB(indexTablaPrimerNivel);

    // ver si esto anda y pasar a funcion
    char *_id = stringID(pcb->id);
    pthread_mutex_lock(&mutex_estados_pcb);
    dictionary_put(estadosPCBS, _id, estado);
    pthread_mutex_unlock(&mutex_estados_pcb);
    free(_id);
    return indexTablaPrimerNivel;
}
char *stringID(uint32_t _id)
{

    char *id = string_new();
    string_append_with_format(&id, "%d", _id);
    return id;
}

t_entradaSegundoNivel *crearEntradaSegundoNivel(uint32_t id, uint32_t * ID_EN_SWAP)
{
    t_entradaSegundoNivel *entrada = malloc(sizeof(t_entradaSegundoNivel));
    entrada->marco = TAM_MEMORIA;
    entrada->modificado = false;
    entrada->presencia = false;
    entrada->uso = false;
    entrada->id = id;
    entrada->paginaSwap = *ID_EN_SWAP;
    log_info(logger, "id_en_swap: %d", *ID_EN_SWAP);
    (*ID_EN_SWAP)++;
    return entrada;
}

t_list *crearTablaSegundoNivel(int entradas, uint32_t id, uint32_t * ID_EN_SWAP)
{
    t_list *tabla = list_create();
    for (int j = 0; j < entradas; j++)
    {
        t_entradaSegundoNivel *entrada = crearEntradaSegundoNivel(id, ID_EN_SWAP);
        list_add(tabla, entrada);
    }
    log_info(logger, "creada tabla de 2do nivel");
    return tabla;
}

t_entradaPrimerNivel *crearEntradaPrimerNivel(int entradasSegundoNivel, uint32_t id, uint32_t * ID_EN_SWAP)
{
    t_list *tablaSegundoNivel = crearTablaSegundoNivel(entradasSegundoNivel, id, ID_EN_SWAP);
    t_entradaPrimerNivel *entrada = malloc(sizeof(t_entradaPrimerNivel));
    pthread_mutex_lock(&mutex_tablas_segundo_nivel);
    entrada->tablaSegundoNivel = list_add(tablasSegundoNivel, tablaSegundoNivel);
    pthread_mutex_unlock(&mutex_tablas_segundo_nivel);
    entrada->id = id;
 
    return entrada;
}

uint32_t obtenerTablaSegundoNivel(uint32_t indexTablaPrimerNivel, uint32_t entradaPagina)
{   
    pthread_mutex_lock(&mutex_tablas_primer_nivel);
    t_list *tablaPrimerNivel = list_get(tablasPrimerNivel, indexTablaPrimerNivel);
    pthread_mutex_unlock(&mutex_tablas_primer_nivel);
    t_entradaPrimerNivel *entrada = list_get(tablaPrimerNivel, entradaPagina);
    return entrada->tablaSegundoNivel;
}

uint32_t obtenerMarco(uint32_t indexTablaSegundoNivel, uint32_t entradaPagina, uint32_t id, bool esWrite)
{
    pthread_mutex_lock(&mutex_entrada_segundo_nivel);
    pthread_mutex_lock(&mutex_tablas_segundo_nivel);
    t_list *tablaSegundoNivel = list_get(tablasSegundoNivel, indexTablaSegundoNivel);
    pthread_mutex_unlock(&mutex_tablas_segundo_nivel);
    
    t_entradaSegundoNivel *entrada = list_get(tablaSegundoNivel, entradaPagina);
    char *_id = stringID(id);
    pthread_mutex_lock(&mutex_estados_pcb);
    t_estadoPCB *estado = dictionary_get(estadosPCBS, _id);
    pthread_mutex_unlock(&mutex_estados_pcb);
    if (!entrada->presencia)
    {
        if (estado->marcosOcupados < MARCOS_POR_PROCESO)
        {
            int marco = -1;
            pthread_mutex_lock(&mutex_bitarray);
            do
            {
                marco++;
            } while (bitarray_test_bit(bitarray, marco));
            bitarray_set_bit(bitarray, marco);
            pthread_mutex_unlock(&mutex_bitarray);
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
                pthread_mutex_lock(&mutex_estados_pcb);
                t_estadoPCB * estadoVictima = dictionary_get(estadosPCBS, _idVictima);
                pthread_mutex_unlock(&mutex_estados_pcb);
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
    pthread_mutex_unlock(&mutex_entrada_segundo_nivel);
    return entrada->marco;
}

void eliminarEntradaSegundoNivel(void *entrada)
{
    if (((t_entradaSegundoNivel *)entrada)->presencia)
    {
        // log_info(logger, "bit:%d valor:%d",((t_entradaSegundoNivel *)entrada)->marco, bitarray_test_bit(bitarray, ((t_entradaSegundoNivel *)entrada)->marco));
        pthread_mutex_lock(&mutex_bitarray);
        bitarray_clean_bit(bitarray, ((t_entradaSegundoNivel *)entrada)->marco);
        pthread_mutex_unlock(&mutex_bitarray);
        // log_info(logger, "bit:%d valor:%d",((t_entradaSegundoNivel *)entrada)->marco, bitarray_test_bit(bitarray, ((t_entradaSegundoNivel *)entrada)->marco));
    }
}

void eliminarEntradaPrimerNivel(void *entrada)
{
    log_info(logger, "eliminado tabla de segundo nivel index:%d", ((t_entradaPrimerNivel *)entrada)->tablaSegundoNivel);
    pthread_mutex_lock(&mutex_tablas_segundo_nivel);
    t_list *tablaSegundoNivel = list_get(tablasSegundoNivel, ((t_entradaPrimerNivel *)entrada)->tablaSegundoNivel);
    pthread_mutex_unlock(&mutex_tablas_segundo_nivel);
    list_iterate(tablaSegundoNivel, eliminarEntradaSegundoNivel);
}

void eliminarMarcos(int indexTablaPrimerNivel)
{
    log_info(logger, "eliminando tablas de primer nivel index: %d", indexTablaPrimerNivel);
    pthread_mutex_lock(&mutex_tablas_primer_nivel);
    t_list *tablaPrimerNivel = list_get(tablasPrimerNivel, indexTablaPrimerNivel);
    pthread_mutex_unlock(&mutex_tablas_primer_nivel);
    pthread_mutex_lock(&mutex_entrada_segundo_nivel);
    list_iterate(tablaPrimerNivel, eliminarEntradaPrimerNivel);
    pthread_mutex_unlock(&mutex_entrada_segundo_nivel);
}

void swapearEntradaSegundoNivel(void *entrada)
{
    t_entradaSegundoNivel *entradaSegundoNivel = (t_entradaSegundoNivel *)entrada;
    if (entradaSegundoNivel->presencia && entradaSegundoNivel->modificado)
    {
        void *marco = leerMarco(entradaSegundoNivel->marco);
        log_info(logger, "se lee el marco:%d", entradaSegundoNivel->marco);
        escribirMarcoSwap(marco, entradaSegundoNivel->paginaSwap, entradaSegundoNivel->id);
        entradaSegundoNivel->presencia = 0;
        entradaSegundoNivel->modificado = 0;
        entradaSegundoNivel->uso = 0;
        pthread_mutex_lock(&mutex_bitarray);
        bitarray_clean_bit(bitarray, entradaSegundoNivel->marco);
        pthread_mutex_unlock(&mutex_bitarray);
        free(marco);
    }
}

void swapearEntradaPrimerNivel(void *entrada)
{
    t_entradaPrimerNivel *entradaPrimerNivel = (t_entradaPrimerNivel *)entrada;
    log_info(logger, "swapeando tabla de segundo nivel index:%d", entradaPrimerNivel->tablaSegundoNivel);
    pthread_mutex_lock(&mutex_tablas_segundo_nivel);
    t_list *tablaSegundoNivel = list_get(tablasSegundoNivel, entradaPrimerNivel->tablaSegundoNivel);
    pthread_mutex_unlock(&mutex_tablas_segundo_nivel);
    list_iterate(tablaSegundoNivel, swapearEntradaSegundoNivel);
}

void suspenderProceso(t_pcb *pcb)
{
    pthread_mutex_lock(&mutex_tablas_primer_nivel);
    t_list *tablaPrimerNivel = list_get(tablasPrimerNivel, pcb->tablaDePaginas);
    pthread_mutex_unlock(&mutex_tablas_primer_nivel);
    log_info(logger, "swapeando tabla de primerNivel nivel index:%d", pcb->tablaDePaginas);
    list_iterate(tablaPrimerNivel, swapearEntradaPrimerNivel);
    char *_id = stringID(pcb->id);
    pthread_mutex_lock(&mutex_estados_pcb);
    t_estadoPCB * estado = dictionary_get(estadosPCBS, _id);
    pthread_mutex_unlock(&mutex_estados_pcb);
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
    pthread_mutex_lock(&mutex_tablas_primer_nivel);
    t_list *tablaPrimerNivel = list_get(tablasPrimerNivel, indexTablaPaginasPrimerNivel);
    pthread_mutex_unlock(&mutex_tablas_primer_nivel);
    t_list *entradasSegundoNivel = list_create();

    void iterarPrimerNivel(t_entradaPrimerNivel * entrada)
    {
        pthread_mutex_lock(&mutex_tablas_segundo_nivel);
        t_list *tablaSegundoNivel = list_get(tablasSegundoNivel, entrada->tablaSegundoNivel);
        pthread_mutex_unlock(&mutex_tablas_segundo_nivel);
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
    log_info(logger, "Esperando retardo memoria de %d milisegundos", RETARDO_MEMORIA/1000);
    usleep(RETARDO_MEMORIA);
}