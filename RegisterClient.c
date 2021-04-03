#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/wait.h> 
#include <signal.h>

#define MAX_MESS 2048
#define MAX_LINE 1024

extern int errno;

int main( int argc, char *argv[]) 
{

  struct addrinfo hints, *res;
  int 			sockfd, n;
  struct sockaddr servaddr;
  socklen_t 		addrlen;
  fd_set			rfds;
char 			net[MAX_LINE], id[MAX_LINE], line[MAX_LINE], command[MAX_LINE], message[MAX_MESS +1];
  enum {notreg, regwait, reg, notregwait} state; 

sockfd = socket(AF_INET, SOCK_DGRAM,0);

memset(&hints, 0, sizeof(hints));
hints.ai_family = AF_INET;
hints.ai_socktype = SOCK_DGRAM;

getaddrinfo(argv[3], argv[4], &hints, &res);

state = notreg;

while(1){

	switch(state){
	
		case notreg:
		printf("Will you join or exit?\n");
		fgets(line, MAX_LINE, stdin);
		sscanf(line, "%s", command);
		if(strcmp(command, "join")==0){
			sscanf(line, "%*s%s%s", net, id);
			sprintf(message, "%s %s %s %s", "REG", net, argv[1], argv[2]);
			sendto(sockfd, message, strlen(message), 0 , res -> ai_addr, res-> ai_addrlen);
			state= regwait;
		} else if (strcmp(command, "exit")==0) {
			freeaddrinfo(res);
			close(sockfd);
			exit(0);
		}
		break;
		
		case regwait:
		printf("Waiting for confirmation ... you can exit\n");
		FD_ZERO(&rfds);
		FD_SET(0, &rfds);
		FD_SET(sockfd, &rfds);
		n = select(sockfd + 1, &rfds, NULL, NULL, (struct timeval *) NULL);
		if(FD_ISSET(sockfd, &rfds)) {
			/* receive message from server*/ 
			addrlen=sizeof(servaddr); 
			n=recvfrom(sockfd, message, MAX_MESS, 0, &servaddr, &addrlen); 
			message[n]='\0'; 
			if(strcmp(message, "OKREG")==0)
			{
				state=reg; 
				printf("Ok you are registered\n"); 
			}
		} 
		else if( FD_ISSET(0, &rfds)) {
		  fgets(line, MAX_LINE, stdin); 
		  sscanf(line, "%s", command); 
		  if(strcmp(command, "exit")==0)
		  {
			  freeaddrinfo(res); 
			  close(sockfd); 
			  exit(0); 
		  }
		}
		break; /* regwait */ 
		
		case reg: 
		printf("Will you leave now? \n"); 
		fgets(line, MAX_LINE, stdin); 
		sscanf(line, "%s", command); 
		if (strcmp(command, "leave")==0){
			sprintf(message, "%s %s %s %s", "UNREG", net, argv[1], argv[2]); 
			n=sendto(sockfd, message, strlen(message), 0, res->ai_addr, res->ai_addrlen); 
			state=notregwait; 
		}
		break; /* reg */ 
		
		case notregwait:
		printf("CHECK\n");
		printf("Waiting for confirmation ... you can exit\n");
		
		/*wait either for confirmation of unregistration or of exit*/
		
		FD_ZERO(&rfds);
		FD_SET(0, &rfds);
		FD_SET(sockfd, &rfds);
		n = select(sockfd + 1, &rfds, NULL, NULL, (struct timeval *) NULL);
		
		if(FD_ISSET(0, &rfds)) {
			fgets(line, MAX_LINE, stdin); 
			sscanf(line, "%s", command); 
			if (strcmp(command, "exit")==0)
			{
				freeaddrinfo(res); 
				close(sockfd); 
				exit(0); 
			} 
		} 
		
		else if( FD_ISSET(sockfd, &rfds)) {
			addrlen=sizeof(servaddr); 
			n=recvfrom(sockfd, message, MAX_MESS, 0, &servaddr, &addrlen); 
			message[n]='\0'; 
			if( (strcmp(message, "OKUNREG")==0))
			{
				state=notreg; 
				printf("Ok, you are not registered\n"); 
			}
		}
		break; /* unregwait */ 	
	}
}
exit(0);
}
