#ifndef STATIC_SOCKETS_SOCKET_H_
#define STATIC_SOCKETS_SOCKET_H_    

    #include<stdio.h>
    #include<stdlib.h>
    #include<sys/socket.h>
    #include<unistd.h>
    #include<netdb.h>
    #include<string.h>
    #include<serializacion/estructuras.h>
    
    uint32_t iniciar_servidor(char* IP, char* PORT);
    uint32_t esperar_cliente(uint32_t socket_servidor);

    uint32_t crear_conexion(char *ip, char* puerto);
    t_paquete* recibirPaquete(uint32_t server_socket);
#endif