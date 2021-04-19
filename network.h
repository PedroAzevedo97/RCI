#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <errno.h> 
#include <unistd.h>
#include <netdb.h> 
#include <signal.h>  
#include <sys/types.h>
#include <sys/socket.h> 
#include <sys/time.h> 
#include <sys/wait.h>
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define MAX_MESS 2048
#define MAX_LINE 1024
#define MAX_BUFFER 128
#define VALUE_NOTREG 1
#define VALUE_REGWAIT 2
#define VALUE_REG 3
#define VALUE_NOTREGWAIT 4

#define JOIN_OK 21
#define CREATE_OK 22
#define GET_OK 23
#define SHOW_OK 24
#define LEAVE_OK 25
#define EXIT_OK 26



int Gethostname(char *name, size_t len); 
int Getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res); 
int Socket(int domain, int type, int protocol); 
ssize_t Sendto(int s, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen); 
ssize_t Recvfrom(int s, void *buf, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen); 
int Close(int fd); 
int Connect(int sockfd, const struct sockaddr *serv_addr, socklen_t addrlen); 
ssize_t Write(int fd, const void *buf, size_t count);
ssize_t Read(int fd, void *buf, size_t count);
int Bind(int sockfd, const struct sockaddr *my_addr, socklen_t addrlen);
int Listen(int sockfd, int backlog);
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int Select(int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
pid_t Fork (void); 
struct hostent *Gethostbyaddr( const void *addr, socklen_t len, int type);
struct hostent *Gethostbyname( const char *name);
//sighandler_t Signal(int signum, sighandler_t handler);
int Shutdown(int sockfd, int type, int protocol);


int UDP_ServerConnection(char *IP, char *port, struct addrinfo **res); 
void Finish(struct addrinfo **res, int *udp_server);  
int Waiting(int *udp_server, struct addrinfo **res); 
int user_interface (char* command, char* IP, char* port, int *fd_UDPServer, int *fd_TCPServer);


/* NEW UDP FUNCTIONS */ 
void UDP_ServerInit (struct addrinfo *hints);
struct addrinfo *UDP_Socket(int *fd_UDPServer, char *IP, char *port, struct addrinfo *hints);
int UDP_ServerAccess(char *port, struct addrinfo *hints);
int UDP_SendMessage(int fd, char* data, int size, struct addrinfo *res);
int UDP_ReceiveMessage(int fd, char* buffer, int size, struct sockaddr_in *addr);
void UDP_AnswerMessage(int fd, char* message, int message_len, struct sockaddr *addr); 


/*TCP Functions */
void TCP_ServerInit (struct addrinfo *hints); 
int TCP_Connection (char *IP, char *port, struct addrinfo *hints, struct addrinfo **res);
int TCP_Read (int fd, char *buffer);
void TCP_Write (int fd, char *data, int size);
