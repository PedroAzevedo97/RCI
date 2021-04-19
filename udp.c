#include "network.h" 


void UDP_ServerInit (struct addrinfo *hints)
{
	memset(hints,0,sizeof(*hints));
	hints.ai_family=AF_INET;//IPv4
	hints.ai_socktype=SOCK_DGRAM;//UDP socket
	hints.ai_flags=AI_PASSIVE;
}


struct addrinfo *UDP_Socket(int *fd_UDPServer, char *IP, char *port, struct addrinfo *hints)
{
	struct addrinfo *res;

	int n=0;
	
	n=Getaddrinfo(IP, port, hints, &res);
	(*fd_UDPServer)=Socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    return res;
}



int UDP_ServerAccess(char *port, struct addrinfo *hints)
{
	struct addrinfo *res;

	int n=0, fd=0;
	
	n=Getaddrinfo(NULL, port, hints, &res);
	fd=Socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	n=Bind(fd, res->ai_addr, res->ai_addrlen);
	
	printf("Access Server Successfully\n");
	return fd; 
	
}

int UDP_SendMessage(int fd, char* data, int size, struct addrinfo *res)
{
	int n=0; 
	n=Sendto(fd, data, size, 0, res->ai_addr, res->ai_addrlen); 
	
	return n; 
}

int UDP_ReceiveMessage(int fd, char* buffer, int size, struct sockaddr_in *addr)
{
	socklen_t addrlen = sizeof(*addr); 
	int n=0;
	
	n=Recvfrom(fd, buffer, size, 0, (struct sockaddr *) addr, &addrlen); 
	buffer[n]='\0'; 
	
	return n; 
}


void UDP_AnswerMessage(int fd, char* message, int message_len, struct sockaddr *addr)
{
	int n=0;
	n=Sendto(fd, message, message_len, 0, addr, sizeof(*addr)); 
}

