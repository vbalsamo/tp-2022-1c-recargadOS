# Libraries
LIBS=static commons pthread

# Custom libraries' paths
SHARED_LIBPATHS=
STATIC_LIBPATHS=../static

# Compiler flags
CDEBUG=-g -Wall -DDEBUG
CRELEASE=-O3 -Wall -DNDEBUG

# Arguments when executing with start, memcheck or helgrind
ARGS=/home/utnso/git/tp-2022-1c-recargadOS/kernel/config/kernel.cfg
# Argumentos Pruebas
BASE=/home/utnso/tp-2022-1c-recargadOS/pruebas/BASE_KERNEL.cfg
PLANI=/home/utnso/tp-2022-1c-recargadOS/pruebas/PLANI_KERNEL.cfg
SUSPE=/home/utnso/tp-2022-1c-recargadOS/pruebas/SUSPE_KERNEL.cfg
# Valgrind flags
MEMCHECK_FLAGS=--track-origins=yes --log-file="memcheck.txt"
HELGRIND_FLAGS=--log-file="helgrind.txt"
