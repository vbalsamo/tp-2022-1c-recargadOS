# Libraries
LIBS=static m commons pthread

# Custom libraries' paths
SHARED_LIBPATHS=
STATIC_LIBPATHS=../static

# Compiler flags
CDEBUG=-g -Wall -DDEBUG
CRELEASE=-O3 -Wall -DNDEBUG

# Arguments when executing with start, memcheck or helgrind
ARGS=/home/juan-dell/git/tp-2022-1c-recargadOS/kernel/config/kernel.cfg
# Argumentos Pruebas
BASE=/home/utnso/tp-2022-1c-recargadOS/pruebas/BASE_CPU.cfg
PLANI=/home/utnso/tp-2022-1c-recargadOS/pruebas/PLANI_CPU.cfg
SUSPE=/home/utnso/tp-2022-1c-recargadOS/pruebas/SUSPE_CPU.cfg
MEMORIA=/home/utnso/tp-2022-1c-recargadOS/pruebas/MEMORIA_CPU.cfg
TLB=/home/utnso/tp-2022-1c-recargadOS/pruebas/TLB_CPU.cfg
INTEGRAL=/home/utnso/tp-2022-1c-recargadOS/pruebas/INTEGRAL_CPU.cfg
# Valgrind flags
MEMCHECK_FLAGS=--track-origins=yes --log-file="memcheck.log"
HELGRIND_FLAGS=--log-file="helgrind.log"
