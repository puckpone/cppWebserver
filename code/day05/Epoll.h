#pragma once
#include <sys/epoll.h>
#include <vector>

class Epoll
{
private:
    int epfd;
    struct epoll_event *events;
public:
    //构造函数中创建epoll事件表
    //初始化events数组
    Epoll();
    ~Epoll();
    //将fd添加到epoll事件表
    void addFd(int fd, uint32_t op);
    //返回一个存储就绪事件的vector
    void updateChannel(Channel*);
    // std::vector<epoll_event> poll(int timeout = -1);
    std::vector<Channel*> poll(int timeout = -1);
};

