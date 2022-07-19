#include <sockets/socket.h>


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
	

	if(socket_servidor < 0){
		perror("error de creacion de socket");
		exit(EXIT_FAILURE);
	}
	if (setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0){
		perror("setsockopt(SO_REUSEADDR) failed");
		exit(EXIT_FAILURE);
	}
    
	int binded = bind(socket_servidor, server_info->ai_addr, server_info->ai_addrlen);
	
	if(binded == -1){
		perror("error al bindear al socket servidor");
		exit(EXIT_FAILURE);
	}
	
	int listening = listen(socket_servidor, SOMAXCONN);
	
	if(listening == -1){
		perror("error al escuchar en socket servidor");
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(server_info);
	
	//log_trace(logger, "Listo para escuchar a mi cliente");

	return socket_servidor;
}

int esperar_cliente(int socket_servidor)
{
	// Aceptamos un nuevo cliente
	int socket_cliente = accept(socket_servidor, NULL, NULL);
	if(socket_cliente == -1){
		perror("error al aceptar socket_cliente");
		exit(EXIT_FAILURE);
	}
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
int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = IPPROTO_TCP;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family,  server_info->ai_socktype,server_info->ai_flags);
	
	if(socket_cliente == -1){
		perror("error de creacion de socket");
		exit(EXIT_FAILURE);
	}

	int resultado_conexion = connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen);
	
	if(resultado_conexion != 0){
		perror("error de conexión");
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(server_info);

	return socket_cliente;
}