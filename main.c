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
					Finish(&res, &fd_UDPServer); 
				break; /* notreg */
			
			
			case regwait:
				if (Waiting(&fd_UDPServer, &res)==VALUE_REG)
					state=reg;
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
				if (Waiting(&fd_UDPServer, &res)==VALUE_NOTREG)
					state=notreg;
				break; /* notregwait */	
		} 
	}
	
	return EXIT_SUCCESS; 	
}
