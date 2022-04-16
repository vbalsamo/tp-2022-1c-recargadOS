#ifndef STATIC_SOCKETS_SERVER_H_
#define STATIC_SOCKETS_SERVER_H_    

    #include<stdio.h>
    #include<stdlib.h>
    #include<sys/socket.h>
    #include<unistd.h>
    #include<netdb.h>
    #include<string.h>
    #include<serializacion/estructuras.h>
    
    int iniciar_servidor(char* IP, char* PORT);
    int esperar_cliente(int socket_servidor);
    t_paquete* recibirPaquete(int server_socket);
#endif