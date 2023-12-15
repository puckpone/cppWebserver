#pragma once
#include <arpa/inet.h>

class InetAddress
{
public:
    struct sockaddr_in addr;
    socklen_t addr_len;
    InetAddress();
    //将ip和port赋值到addr
    InetAddress(const char* ip, uint16_t port);
    ~InetAddress();
};

