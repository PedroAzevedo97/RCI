#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>



#include "../lib/header.h"

#define MAX_MESS 2048
#define MAX_LINE 1024

extern int errno;

int main( int argc, char *argv[]) {

struct addrinfo hint, *res;
int 			sockfd, n;
struct sockaddr servaddr;
socklen_t 		addrlen;
fd_set			rfds;
char 			net[MAX_LINE], id[MAX_LINE], line[MAX_LINE], command[MAX_LINE], message[MAX_MESS +1];

sockfd = Socket(AF_INET, SOCK_DGRAM,0);

bzero(&hints, sizeof(hints));
hints.ai_family = AF_INET;
hints.ai_socktype = SOCK_DGRAM;

Getaddrinfo(argv[3], argv[4], &hints, &res);

state = notreg;

while(1){

	switch(state)[
	
		case notreg:
		printf("Will you join or exit?\n");
		fgets(line, MAX_LINE, stdin);
		sscanf(line, "%s", command);
		if(strcmp(command, "join")==0){
			if(sscanf(line, "%*s%s%s", net, id) != 2)
			break;
			
			sprint(message, "%s %s %s %s", "REG", net, argv[1], argv[2]);
			n = Sendto( sockfd, message, strlen(message), 0 , res -> ai_addr, res-> ai_addrrlen);
			state= regwait;
		
		} else if (strcmp(command, "exit")==0) {
			freeaddrinfo(res);
			Close(sockfd);
			exit(0);
		
		}
		
		break;
		
		case regwait:
		printf("Waiting for confirmation ... you can exit\n");
		
		FD_ZERO(&rfds);
		FD_SET(0, &rfds);
		FD_SET(sockfd, &rfds);
		n = Select(sockfd + 1, &rfds, NULL, NULL, (struct timeval *) NULL);
		if(FD_ISSET(0, &rfds)) {
		} else if( FD_ISSET(sockfd, &rfds)) {
		}
		break;
		
	}
}
exit(0);
}
