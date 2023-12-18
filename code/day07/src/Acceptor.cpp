#include "Acceptor.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "Server.h"

Acceptor::Acceptor(EventLoop *_loop) : loop(_loop)
{
    sock = new Socket();
    addr = new InetAddress("127.0.0.1", 8888);
    sock->bind(addr);
    sock->listen(); 
    sock->setnonblocking();
    acceptChannel = new Channel(loop, sock->getFd());
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
    acceptChannel->setCallback(cb);
    acceptChannel->enableReading();
}

Acceptor::~Acceptor(){
    delete sock; 
    delete addr;
    delete acceptChannel;
}

void Acceptor::acceptConnection(){  
    // 相当于调用Server::newConnection
    newConnectionCallback(sock);
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket*)> _cb){  
    //将Server::newConnection传到这里 并赋给newConnectionCallback
    newConnectionCallback = _cb;
}