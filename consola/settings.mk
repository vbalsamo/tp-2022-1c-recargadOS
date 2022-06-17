# Libraries
LIBS=commons static

# Custom libraries' paths
SHARED_LIBPATHS=
STATIC_LIBPATHS=../static

# Compiler flags
CDEBUG=-g -Wall -DDEBUG
CRELEASE=-O3 -Wall -DNDEBUG

# Arguments when executing with start, memcheck or helgrind
ARGS=/home/utnso/kiss-pruebas/BASE_1 10 /home/utnso/tp-2022-1c-recargadOS/consola/config/consola.cfg

# Arguments when 

BASE1=/home/utnso/tp-2022-1c-recargadOS/pruebas/BASE_1 800 /home/utnso/tp-2022-1c-recargadOS/consola/config/consola.cfg
BASE2=/home/utnso/tp-2022-1c-recargadOS/pruebas/BASE_2 10 /home/utnso/tp-2022-1c-recargadOS/consola/config/consola.cfg
PLANI1=/home/utnso/tp-2022-1c-recargadOS/pruebas/PLANI_1 10 /home/utnso/tp-2022-1c-recargadOS/consola/config/consola.cfg
PLANI2=/home/utnso/tp-2022-1c-recargadOS/pruebas/PLANI_2 10 /home/utnso/tp-2022-1c-recargadOS/consola/config/consola.cfg
SUSPE1=/home/utnso/tp-2022-1c-recargadOS/pruebas/SUSPE_1 10 /home/utnso/tp-2022-1c-recargadOS/consola/config/consola.cfg
SUSPE2=/home/utnso/tp-2022-1c-recargadOS/pruebas/SUSPE_2 10 /home/utnso/tp-2022-1c-recargadOS/consola/config/consola.cfg
SUSPE3=/home/utnso/tp-2022-1c-recargadOS/pruebas/SUSPE_3 10 /home/utnso/tp-2022-1c-recargadOS/consola/config/consola.cfg
# Valgrind flags
MEMCHECK_FLAGS=--track-origins=yes --log-file="memcheck.log"
HELGRIND_FLAGS=--log-file="helgrind.log"
