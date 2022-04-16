#include <main.h>
int main(int argc, char* argv[]) {

    validarParametros(argc, argv);
    t_list* instrucciones = obtenerInstrucciones(argv[1]);
    

    return 0;
}
