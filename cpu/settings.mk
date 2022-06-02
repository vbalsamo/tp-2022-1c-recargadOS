# Libraries
LIBS=pthread static commons

# Custom libraries' paths
SHARED_LIBPATHS=
STATIC_LIBPATHS=../static

# Compiler flags
CDEBUG=-g -Wall -DDEBUG
CRELEASE=-O3 -Wall -DNDEBUG

# Arguments when executing with start, memcheck or helgrind
ARGS=/home/juan-dell/git/tp-2022-1c-recargadOS/kernel/config/kernel.cfg
# Argumentos Pruebas
BASE=/home/utnso/kiss-pruebas/BASE_CPU.cfg
PLANI=/home/utnso/kiss-pruebas/PLANI_CPU.cfg
SUSPE=/home/utnso/kiss-pruebas/SUSPE_CPU.cfg
# Valgrind flags
MEMCHECK_FLAGS=--track-origins=yes --log-file="memcheck.txt"
HELGRIND_FLAGS=--log-file="helgrind.txt"
