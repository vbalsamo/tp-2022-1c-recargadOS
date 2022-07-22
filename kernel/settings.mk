# Libraries
LIBS=static commons pthread

# Custom libraries' paths
SHARED_LIBPATHS=
STATIC_LIBPATHS=../static

# Compiler flags
CDEBUG=-g -Wall -DDEBUG
CRELEASE=-O3 -Wall -DNDEBUG

# Arguments when executing with start, memcheck or helgrind
ARGS=/home/utnso/tp-2022-1c-recargadOS/pruebas/MEMORIA_KERNEL.cfg /home/utnso/tp-2022-1c-recargadOS/pruebas/IP.cfg
# Argumentos Pruebas
BASE=/home/utnso/tp-2022-1c-recargadOS/pruebas/BASE_KERNEL.cfg /home/utnso/tp-2022-1c-recargadOS/pruebas/IP.cfg
PLANI=/home/utnso/tp-2022-1c-recargadOS/pruebas/PLANI_KERNEL.cfg /home/utnso/tp-2022-1c-recargadOS/pruebas/IP.cfg
SUSPE=/home/utnso/tp-2022-1c-recargadOS/pruebas/SUSPE_KERNEL.cfg /home/utnso/tp-2022-1c-recargadOS/pruebas/IP.cfg
MEMORIA=/home/utnso/tp-2022-1c-recargadOS/pruebas/MEMORIA_KERNEL.cfg /home/utnso/tp-2022-1c-recargadOS/pruebas/IP.cfg
TLB=/home/utnso/tp-2022-1c-recargadOS/pruebas/TLB_KERNEL.cfg /home/utnso/tp-2022-1c-recargadOS/pruebas/IP.cfg
INTEGRAL=/home/utnso/tp-2022-1c-recargadOS/pruebas/INTEGRAL_KERNEL.cfg /home/utnso/tp-2022-1c-recargadOS/pruebas/IP.cfg
# Valgrind flags
MEMCHECK_FLAGS=--track-origins=yes --log-file="memcheck.log"
HELGRIND_FLAGS=--log-file="helgrind.txt"
