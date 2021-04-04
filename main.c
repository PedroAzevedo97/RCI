#include "network.h" 

int main (int argc, char *argv[]) 
{
	/*VARIABLES*/ 
	char *IP=NULL, *TCP=NULL, *regIP=NULL, *regUDP=NULL; 
	int fd_UDPServer; 
	
	
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
	fd_UDPServer=UDP_ServerConnection(regUDP); 
	
	return EXIT_SUCCESS; 	
}
