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
int UDP_ServerConnection (char *port); 
