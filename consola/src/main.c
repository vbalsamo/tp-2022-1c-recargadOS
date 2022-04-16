#include <main.h>
int main(int argc, char* argv[]) {

    validarParametros(argc, argv);
    printf("\n%s\n%s\n",argv[1],argv[2]);
    FILE* fileInstrucciones = fopen(argv[1],"r");
    long length = ftell(fileInstrucciones);
    char * stringInstrucciones=malloc(sizeof(char)*(length+1));
    fgets(stringInstrucciones,length+1,fileInstrucciones);

    t_list* instrucciones = obtenerInstrucciones(stringInstrucciones);
    
    return 0;
}
