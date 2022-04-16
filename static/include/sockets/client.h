#ifndef STATIC_SOCKETS_CLIENT_H_
#define STATIC_SOCKETS_CLIENT_H_

    #include<stdio.h>
    #include<stdlib.h>
    #include<unistd.h>
    #include<sys/socket.h>
    #include<netdb.h>
    #include<string.h>

    int crear_conexion(char *ip, char* puerto);
#endif  