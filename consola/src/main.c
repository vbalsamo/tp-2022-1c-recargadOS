#include <main.h>

#define BUFFSIZE 100
int main(int argc, char* argv[]) {

    validarParametros(argc, argv);
    //limpiar esto
    FILE* fileInstrucciones = fopen(argv[1],"r");
    char* stringInstrucciones = string_new();
    char* instruccionActual = malloc(sizeof(char)*BUFFSIZE);
    fgets(instruccionActual,BUFFSIZE,fileInstrucciones);
    while(!feof(fileInstrucciones)){
        string_append(&stringInstrucciones,instruccionActual);
        instruccionActual=fgets(instruccionActual,BUFFSIZE,fileInstrucciones);
        printf("\n%s",instruccionActual);
    }
    //limpiar esto

    t_list* instrucciones = obtenerInstrucciones(stringInstrucciones);
    t_config* config = config_create("/home/juan-dell/git/tp-2022-1c-recargadOS/consola/config/consola.cfg");
    char * IP_KERNEL = config_get_string_value(config,"IP_KERNEL");
    char * PUERTO_KERNEL = config_get_string_value(config,"PUERTO_KERNEL");
 
    int socket_cliente = crear_conexion(IP_KERNEL, PUERTO_KERNEL);
    

    return 0;
}
