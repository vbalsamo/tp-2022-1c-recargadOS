# Libraries
LIBS=static m commons pthread
# Custom libraries' paths
SHARED_LIBPATHS=
STATIC_LIBPATHS=../static

# Compiler flags
CDEBUG=-g -Wall -DDEBUG
CRELEASE=-O3 -Wall -DNDEBUG

# Arguments when executing with start, memcheck or helgrind
ARGS=/home/utnso/tp-2022-1c-recargadOS/memoria/config/SUSPE_MEMORIA.cfg /home/utnso/tp-2022-1c-recargadOS/pruebas/IP.cfg
# Argumentos Pruebas
BASE=/home/utnso/tp-2022-1c-recargadOS/pruebas/BASE_MEMORIA.cfg /home/utnso/tp-2022-1c-recargadOS/pruebas/IP.cfg
PLANI=/home/utnso/tp-2022-1c-recargadOS/pruebas/PLANI_MEMORIA.cfg /home/utnso/tp-2022-1c-recargadOS/pruebas/IP.cfg
SUSPE=/home/utnso/tp-2022-1c-recargadOS/pruebas/SUSPE_MEMORIA.cfg /home/utnso/tp-2022-1c-recargadOS/pruebas/IP.cfg
MEMORIA=/home/utnso/tp-2022-1c-recargadOS/pruebas/MEMORIA_MEMORIA.cfg /home/utnso/tp-2022-1c-recargadOS/pruebas/IP.cfg
TLB=/home/utnso/tp-2022-1c-recargadOS/pruebas/TLB_MEMORIA.cfg /home/utnso/tp-2022-1c-recargadOS/pruebas/IP.cfg
INTEGRAL=/home/utnso/tp-2022-1c-recargadOS/pruebas/INTEGRAL_MEMORIA.cfg /home/utnso/tp-2022-1c-recargadOS/pruebas/IP.cfg

# Valgrind flags
MEMCHECK_FLAGS=--track-origins=yes --log-file="memcheck.log"
HELGRIND_FLAGS=--log-file="helgrind.log"
