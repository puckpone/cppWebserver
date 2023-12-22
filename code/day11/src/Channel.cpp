#include "Channel.h"
#include "EventLoop.h"
#include <unistd.h>

Channel::Channel(EventLoop *_loop, int _fd) 
    : loop(_loop), fd(_fd), events(0), ready(0), inEpoll(false), useThreadPool(true){}

Channel::~Channel(){
    if(fd != -1){
        close(fd);
        fd = -1;
    }
}

void Channel::handleEvent(){
    if(ready & (EPOLLIN | EPOLLPRI)){ //ET模式下，只有 可读|紧急可读 事件才会触发 
        if(useThreadPool)       
            loop->addThread(readCallback);
        else
            readCallback();
    }
    if(ready & (EPOLLOUT)){ //
        if(useThreadPool)       
            loop->addThread(writeCallback);
        else
            writeCallback();
    }
}

void Channel::enableRead(){
    events |= EPOLLIN | EPOLLPRI;
    loop->updateChannel(this);
}

void Channel::useET(){ //使用ET模式
    events |= EPOLLET;
    loop->updateChannel(this);
}

int Channel::getFd(){
    return fd;
}

uint32_t Channel::getEvents(){
    return events;
}


bool Channel::getInEpoll(){
    return inEpoll;
}

void Channel::setInEpoll(bool _in){
    inEpoll = _in;
}


void Channel::setReady(uint32_t _ev){
    ready = _ev;
}

void Channel::setReadCallback(std::function<void()> _cb){
    readCallback = _cb;
}

void Channel::setUseThreadPool(bool use){
    useThreadPool = use;
}
