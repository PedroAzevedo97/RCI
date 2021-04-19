#include "network.h" 

void TCP_ServerInit (struct addrinfo *hints)
{
	memset(hints,0,sizeof(*hints));
	hints.ai_family=AF_INET;//IPv4
	hints.ai_socktype=SOCK_STREAM;//TCP socket
	hints.ai_flags=AI_PASSIVE;
}


int TCP_Connection (char *IP, char *port, struct addrinfo *hints, struct addrinfo **res)
{
	int n=0, fd_TCPServer=0;
	
	n=Getaddrinfo(IP, port, hints, res);
	
	fd_TCPServer=Socket((*res)->ai_family, (*res)->ai_socktype, (*res)->ai_protocol);
	
	n=Connect(fd_TCPServer, (*res)->ai_addr, (*res)->ai_addrlen);
	
	return fd_TCPServer; 
}


int TCP_Read (int fd, char *buffer)
{
	char aux[MAX_BUFFER]; 
	int n=0; 
	
	n=Read(fd, aux, sizeof(aux)); 
	aux[n]='\0';
	strcat(buffer, aux); 
	
	return n; 
}


void TCP_Write (int fd, char *data, int size)
{
	int nSent=0, nByte=0, nLeft=0; 
	
	nByte=size; 
	nLeft=nByte; 
	
	while(nLeft>0)
	{
		nSent=Write(fd, data, nLeft); 
		nLeft-=nSent; 
		data+=nSent; 
	}

	data-=nByte; 
}
