#include <utils/handshake_memoria.h>

t_traduccion_direcciones* obtenerTraduccionDeDirecciones(uint32_t socket){
    t_mensaje * mensaje = malloc(sizeof(t_mensaje));
    mensaje->texto=string_new();
    string_append(&mensaje->texto,"hola");
    mensaje->longitud=strlen(mensaje->texto)+1;
    t_paquete * paquete = armarPaqueteCon(mensaje,REQ_TRADUCCION_DIRECCIONES);
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