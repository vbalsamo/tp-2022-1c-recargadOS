# Libraries
LIBS=commons static

# Custom libraries' paths
SHARED_LIBPATHS=
STATIC_LIBPATHS=../static

# Compiler flags
CDEBUG=-g -Wall -DDEBUG
CRELEASE=-O3 -Wall -DNDEBUG

# Arguments when executing with start, memcheck or helgrind
ARGS=/home/utnso/kiss-pruebas/BASE_1 10 /home/utnso/tp-2022-1c-recargadOS/pruebas/IP.cfg

# Arguments when 

BASE1=/home/utnso/tp-2022-1c-recargadOS/pruebas/BASE_1 64 /home/utnso/tp-2022-1c-recargadOS/pruebas/IP.cfg
BASE2=/home/utnso/tp-2022-1c-recargadOS/pruebas/BASE_2 128 /home/utnso/tp-2022-1c-recargadOS/pruebas/IP.cfg
PLANI1=/home/utnso/tp-2022-1c-recargadOS/pruebas/PLANI_1 64 /home/utnso/tp-2022-1c-recargadOS/pruebas/IP.cfg
PLANI2=/home/utnso/tp-2022-1c-recargadOS/pruebas/PLANI_2 128 /home/utnso/tp-2022-1c-recargadOS/pruebas/IP.cfg
SUSPE1=/home/utnso/tp-2022-1c-recargadOS/pruebas/SUSPE_1 64 /home/utnso/tp-2022-1c-recargadOS/pruebas/IP.cfg
SUSPE2=/home/utnso/tp-2022-1c-recargadOS/pruebas/SUSPE_2 128 /home/utnso/tp-2022-1c-recargadOS/pruebas/IP.cfg
SUSPE3=/home/utnso/tp-2022-1c-recargadOS/pruebas/SUSPE_3 192 /home/utnso/tp-2022-1c-recargadOS/pruebas/IP.cfg
MEMORIA1=/home/utnso/tp-2022-1c-recargadOS/pruebas/MEMORIA_1 4096 /home/utnso/tp-2022-1c-recargadOS/pruebas/IP.cfg
TLB1=/home/utnso/tp-2022-1c-recargadOS/pruebas/TLB_1 2048 /home/utnso/tp-2022-1c-recargadOS/pruebas/IP.cfg
TLB2=/home/utnso/tp-2022-1c-recargadOS/pruebas/TLB_2 2048 /home/utnso/tp-2022-1c-recargadOS/pruebas/IP.cfg
INTEGRAL1=/home/utnso/tp-2022-1c-recargadOS/pruebas/INTEGRAL_1 2048 /home/utnso/tp-2022-1c-recargadOS/pruebas/IP.cfg
INTEGRAL2=/home/utnso/tp-2022-1c-recargadOS/pruebas/INTEGRAL_2 2048 /home/utnso/tp-2022-1c-recargadOS/pruebas/IP.cfg
INTEGRAL3=/home/utnso/tp-2022-1c-recargadOS/pruebas/INTEGRAL_3 2048 /home/utnso/tp-2022-1c-recargadOS/pruebas/IP.cfg
INTEGRAL4=/home/utnso/tp-2022-1c-recargadOS/pruebas/INTEGRAL_4 2048 /home/utnso/tp-2022-1c-recargadOS/pruebas/IP.cfg
INTEGRAL5=/home/utnso/tp-2022-1c-recargadOS/pruebas/INTEGRAL_5 2048 /home/utnso/tp-2022-1c-recargadOS/pruebas/IP.cfg
# Valgrind flags
MEMCHECK_FLAGS=--track-origins=yes --log-file="memcheck.log"
HELGRIND_FLAGS=--log-file="helgrind.log"
