#include <iostream>
#include <unistd.h>
#include <string.h>
#include "src/util.h"
#include "src/Buffer.h"
#include "src/InetAddress.h"
#include "src/Socket.h"

using namespace std;

int main() {
    Socket *sock = new Socket();
    InetAddress *addr = new InetAddress("127.0.0.1", 1234);
    sock->connect(addr);

    int sockfd = sock->getFd();

    Buffer *sendBuffer = new Buffer();
    Buffer *readBuffer = new Buffer();
    
    while(true){
        sendBuffer->getline(); //从标准输入读取一行
        ssize_t write_bytes = write(sockfd, sendBuffer->c_str(), sendBuffer->size());  //写入socket
        if(write_bytes == -1){
            printf("socket already disconnected, can't write any more!\n");
            break;
        }
        int already_read = 0;
        char buf[1024];    //这个buf大小无所谓
        while(true){
            bzero(&buf, sizeof(buf));
            ssize_t read_bytes = read(sockfd, buf, sizeof(buf)); //从socket读取数据
            if(read_bytes > 0){
                readBuffer->append(buf, read_bytes); //写入readBuffer
                already_read += read_bytes;
            } else if(read_bytes == 0){         //EOF
                printf("server disconnected!\n");
                exit(EXIT_SUCCESS);
            }
            if(already_read >= sendBuffer->size()){
                printf("message from server: %s\n", readBuffer->c_str());  //读取完毕，打印readBuffer
                break;
            } 
        }
        readBuffer->clear(); //清空readBuffer
    }
    delete addr;
    delete sock;
    return 0;
}
