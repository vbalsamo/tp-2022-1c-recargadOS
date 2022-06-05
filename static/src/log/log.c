#include <log/log.h>

t_log* iniciar_logger(char* log_path, char* nombre_proceso)
{
    t_log* nuevo_logger;
    if((nuevo_logger = log_create(log_path, nombre_proceso, 1, LOG_LEVEL_INFO)) == NULL){
        printf("No se pudo crear el log\n");
        exit(1);
    }
    return nuevo_logger;
}
char * obtenerPathLog() {
    int MAX_BUF = 200;
    char * time = temporal_get_string_time("%d-%m-%y_%H:%M:%S:%MS");
    char cwd[MAX_BUF];

    getcwd(cwd, MAX_BUF);
    char * directoryPath = string_new();
    char * finalPath = string_new();
    string_append_with_format(&directoryPath, "%s/logs/",cwd);
    string_append_with_format(&finalPath, "%s%s.log",directoryPath, time);
    free(time);
    
    mkdir(directoryPath,S_IRWXU);
    free(directoryPath);
    return finalPath;
}
