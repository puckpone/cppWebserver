#include "ThreadPool.h"

ThreadPool::ThreadPool(int size) : stop(false){
    for(int i = 0; i < size; ++i){  //  启动size个线程
        threads.emplace_back(std::thread([this](){  //定义每个线程的工作函数
            while(true){    
                std::function<void()> task;
                {   //在这个{}作用域内对std::mutex加锁，出了作用域会自动解锁，不需要调用unlock()
                    std::unique_lock<std::mutex> lock(tasks_mtx); //定义一个独占的锁，锁住任务队列 
                    cv.wait(lock, [this](){     //等待条件变量，条件为任务队列不为空或线程池停止
                        return stop || !tasks.empty();  
                        //返回true时，wait()返回，继续执行后面的代码 返回false时，wait()阻塞，等待条件变量被唤醒
                    });
                    if(stop && tasks.empty()) return;   //任务队列为空并且线程池停止，退出线程
                    task = tasks.front();   //从任务队列头取出一个任务
                    tasks.pop();
                }
                task();     //执行任务
            }
        }));
    }
}

ThreadPool::~ThreadPool(){
    {
        std::unique_lock<std::mutex> lock(tasks_mtx);
        stop = true;
    }
    cv.notify_all();
    for(std::thread &th : threads){ //等待线程结束 
        if(th.joinable()) //判断线程是否可连接，可连接就等待线程结束
            th.join(); //阻塞主线程，直到子线程执行完毕
    }
}

void ThreadPool::add(std::function<void()> func){
    { //在这个{}作用域内对std::mutex加锁，出了作用域会自动解锁，不需要调用unlock()
        std::unique_lock<std::mutex> lock(tasks_mtx);
        if(stop)
            throw std::runtime_error("ThreadPool already stop, can't add task any more");
        tasks.emplace(func);
    }
    cv.notify_one();    //通知一次条件变量
}