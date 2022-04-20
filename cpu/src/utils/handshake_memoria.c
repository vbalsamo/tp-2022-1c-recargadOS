#include <utils/handshake_memoria.h>

t_traduccion_direcciones* obtenerTraduccionDeDirecciones(uint32_t socket){
    
    t_paquete * paquete = armarPaqueteCon("",REQ_TRADUCCION_DIRECCIONES);
    enviarPaquete(paquete,socket);
    eliminarPaquete(paquete);
    

    //respuesta
    paquete = recibirPaquete(socket);
    if(paquete->codigo_operacion!=RES_TRADUCCION_DIRECCIONES){
        perror("No se recibio el codigo de operacion esperado para traduccion de direcciones");
        exit(EXIT_FAILURE);
    }
    t_traduccion_direcciones* traduccion_direcciones = deserializarTraduccionDirecciones(paquete->buffer->stream);

    eliminarPaquete(paquete);

    return traduccion_direcciones;


}