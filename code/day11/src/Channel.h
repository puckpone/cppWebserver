#pragma once
#include <sys/epoll.h>
#include <functional>
class EventLoop;
class Channel
{
private:
    EventLoop *loop;
    int fd;
    uint32_t events;
    uint32_t ready;
    bool inEpoll;
    bool useThreadPool;  //是否使用线程池
    std::function<void()> readCallback;
    std::function<void()> writeCallback;
public:
    Channel(EventLoop *_loop, int _fd);
    ~Channel();

    void handleEvent();
    void enableRead();

    int getFd();
    uint32_t getEvents();
    uint32_t getRevents();
    bool getInEpoll();
    void setInEpoll(bool _in = true);
    void useET();

    void setReady(uint32_t _ev);
    void setReadCallback(std::function<void()>);
    void setUseThreadPool(bool use);
};

