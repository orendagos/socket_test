
    /* File Name: client.c */  
      
    #include<stdio.h>  
    #include<stdlib.h>  
    #include<string.h>  
    #include<errno.h>  
    #include<sys/types.h>  

	#include "orenda_socket.h"

      
    #define MAXLINE 4096  
    #define INADDR_LOOPBACK ((u_long)0x7f000001)

	int socket_connect(m_socket_info_t* m_socket_info){
	    printf("%s entry\n", __func__);
		if( connect(m_socket_info->sockfd, (struct sockaddr*)&(m_socket_info->servaddr), sizeof(m_socket_info->servaddr)) < 0){  
				printf("connect error: %s(errno: %d)\n",strerror(errno),errno);  
				return -1;  
				}  
		printf("%s\n", __func__);
		return 0;

	}
    int socket_init(m_socket_info_t* m_socket_info){
		int ret = 0;
    	if( (m_socket_info->sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){  
        printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);  
        exit(0);  
        }  
		memset(&(m_socket_info->servaddr), 0, sizeof(m_socket_info->servaddr));  
        m_socket_info->servaddr.sin_family = AF_INET;  
        m_socket_info->servaddr.sin_port = htons(8000);  
        m_socket_info->servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        /*
        if( inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0){  
        printf("inet_pton error for %s\n",argv[1]);  
        exit(0);  
        }  
        */
		ret = socket_connect(m_socket_info);
		
		return ret;
		
	}

	int socket_send(m_socket_info_t* m_socket_info, void* buf, size_t len){
		if( send(m_socket_info->sockfd, (char*)buf, len, 0) < 0)  
        {  
        printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);  
         return -1; 
        }
		return 0;
	}
	int socket_recv(m_socket_info_t* m_socket_info){
		int rec_len;
		if((rec_len = recv(m_socket_info->sockfd, m_socket_info->buf, MAXLINE,0)) == -1) {  
				   printf("recv error");  
				   return -1; 
				}
		printf("end to socket_recv\n");
		return rec_len;

	}
	int socket_deinit(m_socket_info_t* m_socket_info){
		close(m_socket_info->sockfd); 
		return 0;

	}

	static void *_client_recv_loop(void *handle){
		int ret;
		m_socket_info_t* m_socket_info = (m_socket_info_t*)handle;
		while(1){
			printf("begin to socket_recv\n");
			ret = socket_recv(m_socket_info);
			printf("ret:%d,recv:%s\n", ret, m_socket_info->buf);

		}
		

	}
	
	int send_and_recv_msg_once()  
		{  
			int ret;
			int retry = 0;
			char buf[] = "orenda_socket_info";
			m_socket_info_t* m_socket_info;
			//extern void *malloc(unsigned int num_bytes);

			pthread_t m_client_recv_thread;

			
			m_socket_info = malloc(sizeof(m_socket_info_t));
			if(m_socket_info == NULL){
				printf("malloc fail\n");
				return -1;
			}
			ret = socket_init(m_socket_info);
			ret = pthread_create(&m_client_recv_thread, NULL, _client_recv_loop, (void *)m_socket_info);
    if(ret != 0) {
        printf("can't create thread (%d:%s)", ret, strerror(ret));
        ret = -1;
    }
				//ret = socket_connect(m_socket_info);// ??? i guess that it can be true ,or why set already connected?
				//fgets(buf,MAXLINE, stdin);//char *fgets(char *str, int n, FILE *stream);
				//if(strcmp(buf, "q\n") == 0) return -1;
			//ret = socket_send(m_socket_info, buf, sizeof(buf));
			//ret = socket_recv(m_socket_info);
			//printf("ret:%d,recv:%s\n", ret, m_socket_info->buf);
			//usleep(1000*1000*20);
			while(retry<5){
				//usleep(1000*1000*2);
				fgets(buf,MAXLINE, stdin);//char *fgets(char *str, int n, FILE *stream);
				if(strcmp(buf, "q\n") == 0) return -1;
				//strcpy(buf, "client_send_msg\n");
				printf("SEND:%s\n", buf);
				ret = socket_send(m_socket_info, buf, sizeof(buf));
				retry++;
			}
			pthread_join(&m_client_recv_thread, NULL);
			ret = socket_deinit(m_socket_info);
			return ret;
		}  
	int main(int argc,char** argv){
		int ret;
		while(1){
			usleep(1000*1000);
			 ret = send_and_recv_msg_once();
			if(ret<0) break;
		}
		return 0;
		

	}
    int main_unsolved(int argc, char** argv)  
    {  
    	int ret;
		char buf[] = "orenda_socket_info";
        m_socket_info_t* m_socket_info;
		//extern void *malloc(unsigned int num_bytes);
		m_socket_info = malloc(sizeof(m_socket_info_t));
		if(m_socket_info == NULL){
		    printf("malloc fail\n");
			return -1;
		}
		ret = socket_init(m_socket_info);
		while(1){
			//ret = socket_connect(m_socket_info);// ??? i guess that it can be true ,or why set already connected?
			fgets(buf,MAXLINE, stdin);//char *fgets(char *str, int n, FILE *stream);
			if(strcmp(buf, "q\n") == 0) break;
		ret = socket_send(m_socket_info, buf, sizeof(buf));
		ret = socket_recv(m_socket_info);
		printf("ret:%d,recv:%s\n", ret, m_socket_info->buf);
			}
		ret = socket_deinit(m_socket_info);
		return ret;
    }  

