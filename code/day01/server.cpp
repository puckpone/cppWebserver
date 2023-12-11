#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <stdio.h>

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); //创建一个套接字
    //创建服务器结构体 
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));  //初始化结构体
    //设置ip、端口
    serv_addr.sin_family = AF_INET;  //ip4
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);
    //将socket地址与文件描述符绑定
    bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));
    
    //监听这个socket端口  SOMAXCONN即最大监听队列长度 128
    listen(sockfd,SOMAXCONN);

    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    bzero(&clnt_addr, sizeof(clnt_addr));
    int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);  //阻塞等待连接
    printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
    return 0;
}


