
/* File Name: server.c */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <pthread.h>
#include "orenda_socket.h"




int server_init(m_socket_server_t* orenda_server_info_t)
{
    if( (orenda_server_info_t->socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }
    //初始化
    memset(&(orenda_server_info_t->servaddr), 0, sizeof(orenda_server_info_t->servaddr));
    orenda_server_info_t->servaddr.sin_family = AF_INET;
    orenda_server_info_t->servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址。
    orenda_server_info_t->servaddr.sin_port = htons(DEFAULT_PORT);//设置的端口为DEFAULT_PORT
    if( bind(orenda_server_info_t->socket_fd, (struct sockaddr*)&(orenda_server_info_t->servaddr), sizeof(orenda_server_info_t->servaddr)) == -1) {
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }
    //开始监听是否有客户端连接
    if( listen(orenda_server_info_t->socket_fd, 10) == -1) {
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }
    printf("======init server successfully, and there is client to be added======\n");
    printf("******attention: connect_fid has not been inited*******\n");
    return 0;

}
int server_recv(m_socket_server_t* orenda_server_info_t)
{
    int n;
    //while(1){
    //阻塞直到有客户端连接，不然多浪费CPU资源。
    if( (orenda_server_info_t->connect_fd = accept(orenda_server_info_t->socket_fd, (struct sockaddr*)NULL, NULL)) == -1) {
        printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
        //continue;
        return -1;
    }
    //接受客户端传过来的数据
    n = recv(orenda_server_info_t->connect_fd, orenda_server_info_t->buff, MAXLINE, 0);
    orenda_server_info_t->buff[n] = '\0';
    printf("recv msg from client: %s\n", orenda_server_info_t->buff);
    //close(connect_fd);//why close connect_fd/ what's the difference between socketid and connect_fd

    //}
    return 0;
}
int server_send(m_socket_server_t* orenda_server_info_t, void* buf, size_t len)
{
    if(orenda_server_info_t->connect_fd == 0) {
        printf("init connect_fd,first\n");
    }
    if(send(orenda_server_info_t->connect_fd, buf, len,0) == -1)  {
        perror("send error");
        close(orenda_server_info_t->connect_fd);
        exit(0);

    }
}
int server_deinit(m_socket_server_t* orenda_server_info_t)
{
    close(orenda_server_info_t->connect_fd);
    close(orenda_server_info_t->socket_fd);
}

static void *_server_thread_loop(void *handle)
{
    int ret;
    char buf[]="server_send_info\n";
    m_socket_server_t* orenda_server_info_t;

    orenda_server_info_t = (m_socket_server_t*)handle;
    while(1) {
        printf("begin to server_recv\n");
        ret = server_recv(orenda_server_info_t);
        if(ret<0){ 
			printf("ret<0\n");
			continue;

		}
        printf("end to server_recv\n");
        ret = server_send(orenda_server_info_t, (char*)buf, sizeof(buf));
        printf("end to server_send\n");
    }
    return NULL;

}


int main()
{
    m_socket_server_t* orenda_server_info_t;
    int ret;
    char buf[]="server_send_info\n";
    pthread_t m_server_thread;

    orenda_server_info_t = malloc(sizeof(m_socket_server_t));
    if(orenda_server_info_t == NULL) {
        printf("orenda_server_info_t malloc failed\n");
        return -1;
    }
    ret = server_init(orenda_server_info_t);

    ret = pthread_create(&m_server_thread, NULL, _server_thread_loop, (void *)orenda_server_info_t);
    if(ret != 0) {
        printf("can't create thread (%d:%s)", ret, strerror(ret));
        ret = -1;
    }
    /*
    	ret = server_recv(orenda_server_info_t);

    	ret = server_send(orenda_server_info_t, (char*)buf, sizeof(buf));
    	*/
    pthread_join(&m_server_thread, NULL);
    ret = server_deinit(orenda_server_info_t);

    return ret;

}

