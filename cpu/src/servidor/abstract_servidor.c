#include <servidor/abstract_servidor.h>
void servidor(void * params){
    t_servidor * servidor = (t_servidor *)params;
    while(1){
        int socket_cliente = esperar_cliente(*servidor->socket);
        log_info(logger, "%s: se conecta cliente", servidor->nombre);
        //instanciar hilo que atienda la solicitud
        t_paquete * paquete = recibirPaquete(socket_cliente);
        servidor->deserializarSegun(paquete, socket_cliente);
    }
}

t_servidor * obtenerServidor(int * socket, void(*deserializarSegun)(t_paquete*,int), char nombre[10]){
    t_servidor * servidor = malloc(sizeof(t_servidor));
    servidor->socket = socket;
    servidor->deserializarSegun=deserializarSegun;
    memcpy(servidor->nombre, nombre, 10);
    return servidor;
}