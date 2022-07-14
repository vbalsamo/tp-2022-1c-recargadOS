#ifndef MEMORIA_SWAP_H_
#define MEMORIA_SWAP_H_

#include <stdlib.h>
#include <stdio.h>
#include <utils/variables_globales.h>
#include <serializacion/estructuras.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <commons/bitarray.h>
#include <math.h>
#include <string.h>
#include <utils/memoria.h>

pthread_mutex_t mutex_swap;
void iniciarEstructurasSwap();
char * obtnerPathSwap(uint32_t id);
void crearArchivoSwap(uint32_t id, uint32_t tamanioProceso);
void escribirMarcoSwap(void * contenidoMarco, uint32_t numeroMarco, uint32_t id);
void * leerPaginaSwap(uint32_t paginaSwap,uint32_t id);
void eliminarArchivoSwap(uint32_t id);
void suspenderProceso (t_pcb * pcb);
void desuspenderProceso (t_pcb * pcb);

#endif