#include <sockets/server.h>


int iniciar_servidor(char* IP, char* PORT)
{
	int socket_servidor;
	struct addrinfo hints, *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = PF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = IPPROTO_TCP;
	
	getaddrinfo(IP, PORT, &hints, &server_info);

	socket_servidor = socket(server_info->ai_family,  server_info->ai_socktype,server_info->ai_flags);
	bind(socket_servidor, server_info->ai_addr, server_info->ai_addrlen);
	listen(socket_servidor, SOMAXCONN);
	
	freeaddrinfo(server_info);
	
	//log_trace(logger, "Listo para escuchar a mi cliente");

	return socket_servidor;
}

int esperar_cliente(int socket_servidor)
{
	// Aceptamos un nuevo cliente
	int socket_cliente = accept(socket_servidor, NULL, NULL);

	return socket_cliente;
}

t_paquete* recibirPaquete(int server_socket){

	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->buffer = malloc(sizeof(t_buffer));

	recv(server_socket, &(paquete->codigo_operacion), sizeof(t_cod_op), 0);

	recv(server_socket, &(paquete->buffer->size), sizeof(uint32_t), 0);

	paquete->buffer->stream = malloc(paquete->buffer->size);

	recv(server_socket, paquete->buffer->stream, paquete->buffer->size, 0);

	return paquete;
}