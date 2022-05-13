#include <sockets/socket.h>


uint32_t iniciar_servidor(char* IP, char* PORT)
{
	uint32_t socket_servidor;
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

uint32_t esperar_cliente(uint32_t socket_servidor)
{
	// Aceptamos un nuevo cliente
	uint32_t socket_cliente = accept(socket_servidor, NULL, NULL);
	t_log * logger = log_create("log.log", "Servidor", 1, LOG_LEVEL_DEBUG);
	log_info(logger, "Se conecto un cliente!");
	return socket_cliente;
}

t_paquete* recibirPaquete(uint32_t server_socket){

	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->buffer = malloc(sizeof(t_buffer));

	recv(server_socket, &(paquete->codigo_operacion), sizeof(t_cod_op), 0);

	recv(server_socket, &(paquete->buffer->size), sizeof(uint32_t), 0);

	paquete->buffer->stream = malloc(paquete->buffer->size);

	recv(server_socket, paquete->buffer->stream, paquete->buffer->size, 0);

	return paquete;
}
uint32_t crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = PF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = IPPROTO_TCP;

	getaddrinfo(ip, puerto, &hints, &server_info);

	uint32_t socket_cliente = socket(server_info->ai_family,  server_info->ai_socktype,server_info->ai_flags);
	
	connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen);

	freeaddrinfo(server_info);

	return socket_cliente;
}