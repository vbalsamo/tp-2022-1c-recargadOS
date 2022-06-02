# Libraries
LIBS=static commons

# Custom libraries' paths
SHARED_LIBPATHS=
STATIC_LIBPATHS=../static

# Compiler flags
CDEBUG=-g -Wall -DDEBUG
CRELEASE=-O3 -Wall -DNDEBUG

# Arguments when executing with start, memcheck or helgrind
ARGS=/home/utnso/tp-2022-1c-recargadOS/memoria/config/memoria.cfg
# Argumentos Pruebas
BASE=/home/utnso/tp-2022-1c-recargadOS/pruebas/BASE_MEMORIA.cfg
PLANI=/home/utnso/tp-2022-1c-recargadOS/pruebas/PLANI_MEMORIA.cfg
SUSPE=/home/utnso/tp-2022-1c-recargadOS/pruebas/SUSPE_MEMORIA.cfg
# Valgrind flags
MEMCHECK_FLAGS=--track-origins=yes --log-file="memcheck.log"
HELGRIND_FLAGS=--log-file="helgrind.log"
