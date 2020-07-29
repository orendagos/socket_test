
#include<sys/socket.h>
#include<netinet/in.h>

#define MAXLINE 4096
#define DEFAULT_PORT 8002


typedef struct orenda_socket_info {
    int    sockfd;
    struct sockaddr_in servaddr;
    char    buf[MAXLINE];
} m_socket_info_t;

typedef struct orenda_server_info {
    int    socket_fd;
    int connect_fd;
    struct sockaddr_in	   servaddr;
    char	buff[MAXLINE];

} m_socket_server_t;
