#ifndef STATIC_SERIALIZACION_PAQUETE_H_
#define STATIC_SERIALIZACION_PAQUETE_H_

    #include<stdio.h>
    #include<stdlib.h>
    
    serializar_paquete(t_paquete* paquete, int bytes);
    enviar_paquete(t_paquete* paquete, int socket_cliente);
    eliminar_paquete(t_paquete* paquete);
#endif  