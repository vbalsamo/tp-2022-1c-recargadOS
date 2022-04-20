# Original project path
PROJ_PATH=..

# Excluded source files (eg: main() function)
EXCLUDE=main.c

# Libraries
LIBS=cspecs static commons

# Custom libraries' paths
SHARED_LIBPATHS=
STATIC_LIBPATHS=../../static

# Compiler flags
CDEBUG=-g -Wall -DDEBUG
CRELEASE=-O3 -Wall -DNDEBUG

# Arguments when executing with start, memcheck or helgrind
ARGS=

# Valgrind flags
MEMCHECK_FLAGS=--track-origins=yes --log-file="memcheck.log"
HELGRIND_FLAGS=--log-file="helgrind.log"
