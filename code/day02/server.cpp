#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include "util.h"
int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); //创建一个套接字
    errif(sockfd==-1, "socket create error\n");
    //创建服务器结构体 
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));  //初始化结构体
    //设置ip、端口
    serv_addr.sin_family = AF_INET;  //ip4
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);
    //将socket地址与文件描述符绑定
    errif(bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr))==-1, "socket bind error\n");
    
    //监听这个socket端口  SOMAXCONN即最大监听队列长度 128
    errif(listen(sockfd,SOMAXCONN), "socket listen error\n");

    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    bzero(&clnt_addr, sizeof(clnt_addr));
    //阻塞等待连接  
    //accept会创建新的socket来表示此次连接，并将该socket的文件描述符返回
    int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);  
    errif(clnt_sockfd == -1, "socket accept error\n");

    printf("new client fd %d! IP: %s Port: %d\n", 
        clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), 
        ntohs(clnt_addr.sin_port));

    char buf[1024];     //定义缓冲区
    while (true) {
        bzero(&buf, sizeof(buf));       //清空缓冲区
        ssize_t read_bytes = read(clnt_sockfd, buf, sizeof(buf)); //从客户端socket读到缓冲区，返回已读数据大小
        if(read_bytes > 0){
            printf("message from client fd %d: %s\n", clnt_sockfd, buf);  
            write(clnt_sockfd, buf, sizeof(buf));           //将相同的数据写回到客户端
        } else if(read_bytes == 0){             //read返回0，表示EOF
            printf("client fd %d disconnected\n", clnt_sockfd);
            close(clnt_sockfd);
            break;
        } else if(read_bytes == -1){        //read返回-1，表示发生错误，按照上文方法进行错误处理
            close(clnt_sockfd);
            errif(true, "socket read error");
        }
    }
    return 0;
}


