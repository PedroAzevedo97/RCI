#include "network.h" 

int Gethostname(char *name, size_t len)
{
	int n; 
	n=gethostname(name, len);
	if(n==-1)
	{
		perror("gethostname"); 
		exit(1); 
	}
	return n; 
}

int Getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res)
{
	int n; 
	n=getaddrinfo(node, service, hints, res);
	if(n==-1)
	{
		perror("getaddrinfo"); 
		exit(1); 
	}
	return n;
}

int Socket(int domain, int type, int protocol)
{
	int n; 
	n=socket(domain, type, protocol); 
	if(n==-1)
	{
		perror("socket"); 
		exit(1); 
	}
	return n;
}

ssize_t Sendto(int s, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen)
{
	ssize_t n; 
	n=sendto(s, buf, len, flags, dest_addr, addrlen); 
	if(n==-1)
	{
		perror("sendto"); 
		exit(1); 
	}
	return n;
}


ssize_t Recvfrom(int s, void *buf, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen)
{
	ssize_t n; 
	n=recvfrom(s, buf, len, flags, from, fromlen); 
	if(n==-1)
	{
		perror("recvfrom"); 
		exit(1); 
	}
	return n;
}

int Close(int fd)
{
	int n; 
	n=close(fd); 
	if(n==-1)
	{
		perror("close"); 
		exit(1); 
	}
	return n;	
}

int Connect(int sockfd, const struct sockaddr *serv_addr, socklen_t addrlen)
{
	int n; 
	n=connect(sockfd, serv_addr, addrlen); 
	if(n==-1)
	{
		perror("connect"); 
		exit(1); 
	}
	return n;
}

ssize_t Write(int fd, const void *buf, size_t count)
{
	ssize_t n; 
	n=write(fd, buf, count); 
	if(n==-1)
	{
		perror("write"); 
		exit(1); 
	}
	return n;
}

ssize_t Read(int fd, void *buf, size_t count)
{
	ssize_t n; 
	n=read(fd, buf, count); 
	if(n==-1)
	{
		perror("read"); 
		exit(1); 
	}
	return n;
}

int Bind(int sockfd, const struct sockaddr *my_addr, socklen_t addrlen)
{
	int n; 
	n=bind(sockfd, my_addr, addrlen); 
	if(n==-1)
	{
		perror("bind"); 
		exit(1); 
	}
	return n;
}

int Listen(int sockfd, int backlog)
{
	int n; 
	n=listen(sockfd, backlog); 
	if(n==-1)
	{
		perror("listen"); 
		exit(1); 
	}
	return n;	
}



int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
	int n; 
	n=accept(sockfd, addr, addrlen); 
	if(n==-1)
	{
		perror("accept"); 
		exit(1); 
	}
	return n;	
}

int Select(int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
{
	int n; 
	n=select(n, readfds, writefds, exceptfds, timeout); 
	if(n<=0)
	{
		perror("select"); 
		exit(1); 
	}
	return n;
}

pid_t Fork (void)
{
	pid_t n; 
	n=fork(); 
	if(n==-1)
	{
		perror("fork"); 
		exit(1); 
	}
	return n;
}


int UDP_ServerConnection(char *IP, char *port, struct addrinfo **res)
{
	int fd, ret; 
	struct addrinfo hints;
	
	fd = Socket(AF_INET, SOCK_DGRAM, 0); 
	
	memset(&hints,0,sizeof hints);
	hints.ai_family=AF_INET;//IPv4
	hints.ai_socktype=SOCK_DGRAM;//UDP socket
	hints.ai_flags=AI_PASSIVE;	
	
	ret = Getaddrinfo(IP, port, &hints, res); 
	ret = Bind(fd, (*res)->ai_addr, (*res)->ai_addrlen); 
	
	return fd;
}


void Finish(struct addrinfo **res, int *udp_server)
{
	freeaddrinfo(*res); 
	Close(*fudp_server); 
	exit(0); 
}

int Waiting(int *udp_server, struct addrinfo **res)
{
	char line[MAX_LINE], command[MAX_LINE], message[MAX_MESS+1]; 
	int ret, state_value=-1;
	fd_set rfds;
	socklen_t addrlen;
	struct sockaddr servaddr;
	ssize_t n;
	
	printf("Waiting for confirmation ... you can exit\n"); 
	
	/* wait either for confirmation of unregistration or for exit */ 
	
	FD_ZERO(&rfds);
	FD_SET(0, &rfds);
	FD_SET(*udp_server, &rfds);
    
    ret = Select((*udp_server) + 1, &rfds, (fd_set *) NULL, (fd_set *) NULL, (struct timeval *) NULL);

	if(FD_ISSET(*udp_server, &rfds)) 
	{
		/* receive confirmation from server*/ 
		addrlen=sizeof(servaddr); 
		n=Recvfrom(*udp_server, message, MAX_MESS, 0, &servaddr, &addrlen); 
		message[n]='\0'; 
		
		if(strcmp(message, "OKREG")==0)
		{
			state_value=VALUE_REG; 
			printf("Ok you are registered\n"); 
		}
		
		else if(strcmp(message, "OKUNREG")==0)
		{
			state_value=VALUE_NOTREG; 
			printf("Ok, you are not registered\n"); 
		}
	}
	
	else if(FD_ISSET(0, &rfds)) 
	{
		fgets(line, MAX_LINE, stdin); 
		sscanf(line, "%s", command); 
		if(strcmp(command, "exit")==0)
			Finish(res, udp_server); 
	}
}
