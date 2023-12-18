#include "Server.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include <functional>
#include <string.h>
#include <unistd.h>
#include <cstdio>
#include <cerrno>
#include <Acceptor.h>
#include <map>
#include "Connection.h"
#define READ_BUFFER 1024

Server::Server(EventLoop *_loop) : loop(_loop){    
     
    acceptor = new Acceptor(loop);
    
           /*std::placeholders::_1: This is a placeholder for the first argument of the function. 
    In this case, it indicates that the Socket* parameter of newConnection will be provided 
    when the std::function is called*/
    std::function<void(Socket*)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);

}

Server::~Server()
{
    
}


void Server::newConnection(Socket *sock){
    Connection *conn = new Connection(loop, sock);
    std::function<void(Socket*)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
    conn->setDeleteConnectionCallback(cb);
    connections[sock->getFd()] = conn;
}

void Server::deleteConnection(Socket * sock){
    Connection *conn = connections[sock->getFd()];
    connections.erase(sock->getFd());
    delete conn;
}