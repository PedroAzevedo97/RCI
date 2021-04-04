#include "network.h" 

int main (int argc, char *argv[]) 
{
	/*VARIABLES*/ 
	char *IP=NULL, *TCP=NULL, *regIP=NULL, *regUDP=NULL; 
	char line[MAX_LINE], command[MAX_LINE], net[MAX_LINE], id[MAX_LINE], message[MAX_MESS+1]; 
	int fd_UDPServer, ret; 
	enum {notreg, regwait, reg, notregwait} state; 
	struct addrinfo *res; 
	fd_set rfds; 
	struct sockaddr servaddr;
	socklen_t addrlen;
	ssize_t n;
	
	
	/*INPUT ERROR TEST*/ 
	if(argc!=5)
	{
		printf("Error: To invoke this program the command is of type: 'ndn IP TCP regIP regUDP \n")
		exit(1);
	} 
	
	/*SAVE ARGUMENTS OF INTEREST*/
	else
	{
		IP= (char*) calloc(strlen(argv[1])+1, sizeof(char)); 
		TCP= (char*) calloc(strlen(argv[2])+1, sizeof(char));
		regIP= (char*) calloc(strlen(argv[3])+1, sizeof(char)); 
		regUDP= (char*) calloc(strlen(argv[4])+1, sizeof(char)); 
		strcpy(IP, argv[1]); 
		strcpy(TCP, argv[2]); 
		strcpy(regIP, argv[3]);
		strcpy(regUDP, argv[4]); 
	}
	
	/* UDP CONNECTION */ 
	fd_UDPServer=UDP_ServerConnection(regIP, regUDP, &res); 
	
	/* STATE MACHINE */ 
	state=notreg; 
	
	while(1)
	{
		switch(state)
		{
			case notreg: 
				printf("Will you join or exit? \n");
				fgets(line, MAX_LINE, stdin); 
				sscanf(line, "%s", command); 
				if(strcmp(command, "join")==0)
				{
					sscanf(line, "%*s%s%s", net, id);
					sprintf(message, "%s %s %s %s", "REG", net, IP, TCP); 
					n=Sendto(fd_UDPServer, message, strlen(message), 0, res -> ai_addr, res-> ai_addrlen); 
					state=regwait; 
				}
				else if(strcmp(command, "exit")==0) 
				{
					freeaddrinfo(res); 
					Close(fd_UDPServer); 
					exit(0); 
				}
				break; /* notreg */
			
			
			case regwait:
				printf("Waiting for confirmation ... you can exit\n"); 
				FD_ZERO(&rfds);
				FD_SET(0, &rfds);
				FD_SET(fd_UDPServer, &rfds);
				ret = Select(fd_UDPServer + 1, &rfds, (fd_set *) NULL, (fd_set *) NULL, (struct timeval *) NULL);
				if(FD_ISSET(fd_UDPServer, &rfds)) 
				{
					/* receive message from server*/ 
					addrlen=sizeof(servaddr); 
					n=Recvfrom(fd_UDPServer, message, MAX_MESS, 0, &servaddr, &addrlen); 
					message[n]='\0'; 
					if(strcmp(message, "OKREG")==0)
					{
						state=reg; 
						printf("Ok you are registered\n"); 
					}
					else if(FD_ISSET(0, &rfds))
					{
						fgets(line, MAX_LINE, stdin); 
						sscanf(line, "%s", command); 
						if(strcmp(command, "exit")==0)
						{
							freeaddrinfo(res); 
							Close(fd_UDPServer); 
							exit(0); 
						}
					}
				}
				break; /*regwait*/
				
			case reg:
				printf("Will you leave now? \n"); 
				fgets(line, MAX_LINE, stdin); 
				sscanf(line, "%s", command); 
				if (strcmp(command, "leave")==0)
				{
					/*unregister the node*/ 
					sprintf(message, "%s %s %s %s", "UNREG", net, IP, TCP); 
					n=Sendto(fd_UDPServer, message, strlen(message), 0, res->ai_addr, res->ai_addrlen); 
					state=notregwait; 
				}
				break; /* reg */ 
			
			case notregwait: 
				printf("Waiting for confirmation ... you can exit\n"); 
				
				/* wait either for confirmation of unregistration or for exit */ 
				FD_ZERO(&rfds);
				FD_SET(0, &rfds);
				FD_SET(fd_UDPServer, &rfds);
				ret = Select(fd_UDPServer + 1, &rfds, (fd_set *) NULL, (fd_set *) NULL, (struct timeval *) NULL);
				if(FD_ISSET(0, &rfds))
				{
					fgets(line, MAX_LINE, stdin); 
					sscanf(line, "%s", command); 
					if(strcmp(command, "exit")==0)
					{
						freeaddrinfo(res); 
						Close(fd_UDPServer); 
						exit(0); 
					}
				}
				
				else if(FD_ISSET(fd_UDPServer, &rfds))
				{
					/*receive confirmation */
					addrlen=sizeof(servaddr); 
					n=Recvfrom(fd_UDPServer, message, MAX_MESS, 0, &servaddr, &addrlen); 
					message[n]='\0'; 
					if(strcmp(message, "OKUNREG")==0)
					{
						state=notreg; 
						printf("Ok, you are not registered\n"); 
					}
				}	
				break; /* notregwait */	
		} 
	}
	
	return EXIT_SUCCESS; 	
}
