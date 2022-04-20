#include <servidor/abstract_servidor.h>
void servidor(void * params){
    t_servidor * servidor = (t_servidor *)params;
    while(1){
        uint32_t socket_cliente = esperar_cliente(*servidor->socket);

        //instanciar hilo que atienda la solicitud
        t_paquete * paquete = recibirPaquete(socket_cliente);
        servidor->deserializarSegun(paquete, socket_cliente);
    }
}
t_servidor * obtenerServidor(uint32_t * socket, void(*deserializarSegun)(t_paquete*,uint32_t)){
    t_servidor * servidor = malloc(sizeof(t_servidor));
    servidor->socket = socket;
    servidor->deserializarSegun=deserializarSegun;
    return servidor;
}