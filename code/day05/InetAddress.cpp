#include "InetAddress.h"
#include <string.h>
//在进入构造函数之前对成员变量addr_len初始化
InetAddress::InetAddress() : addr_len(sizeof(addr)){
    bzero(&addr, sizeof(addr));
}
InetAddress::InetAddress(const char* ip, uint16_t port) : addr_len(sizeof(addr)){
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
    addr_len = sizeof(addr);
}

InetAddress::~InetAddress(){
}
