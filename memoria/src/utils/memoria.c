#include <utils/memoria.h>


void iniciarEstructurasMemoria(void) {
    memoria = malloc(TAM_MEMORIA);
    tablas = malloc(sizeof(t_tablas));
    tablas->tablasPrimerNivel = list_create();
    tablas->tablasSegundoNivel = list_create();
}