#pragma once
#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>

class ThreadPool
{
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks; // task function queue
    std::mutex tasks_mtx;
    std::condition_variable cv;
    bool stop;
public:
    ThreadPool(int size = 10);
    ~ThreadPool();

    // void add(std::function<void()>);
    template<class F, class... Args>
    auto add(F&& f, Args&&... args) 
    -> std::future<typename std::result_of<F(Args...)>::type>;
};


    /*
auto ThreadPool::add(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>
    std::result_of 是一个 C++11 引入的元编程工具，用于获取调用函数类型的返回类型
    std::result_of<F(Args...)>::type 表示调用函数F的返回类型
    F(Args...) 表示用参数 Args... 调用函数对象 F
    std::future<typename std::result_of<F(Args...)>::type>:
        std::future 是 C++11 引入的异步编程工具，表示一个未来的值或异步操作的结果。
        typename std::result_of<F(Args...)>::type 表示 std::future 将持有的值的类型。
    
    '->'  为尾置返回值类型 表示 add的返回值类型为 std::future<typename std::result_of<F(Args...)>::type>

using return_type = typename std::result_of<F(Args...)>::type; 
    typename std::result_of<F(Args...)>::type：这是使用 std::result_of 元编程工具获取函数调用的返回类型的方式。
    typename 是必需的，因为 std::result_of<F(Args...)>::type 是一个依赖类型，需要使用 typename 来告诉编译器这是一个类型。

    using return_type = ...：这是类型别名的语法，表示将右侧的类型表达式赋值给左侧的标识符 return_type。
    这样，你可以使用 return_type 代替较长的 std::result_of<F(Args...)>::type


    std::forward<F>(f) 表示完美转发 ：
        在传递参数的时候，保持原来
        的参数类型（左引用保持左引用，右引用保持右引用），这样可以避免不必要的拷贝和转换，提高效率。

    std::bind(std::forward<F>(f), std::forward<Args>(args)...) 表示将函数f和参数args绑定
    std::packaged_task 是一个模板类，它的模板参数是一个函数类型，它通过 std::bind 将函数和参数绑定，然后将函数的返回值传递给 std::future 对象
    
    std::make_shared< std::packaged_task<return_type()> >() 表示创建一个 std::packaged_task 对象
    std::make_shared 会分配创建传入参数中的对象，并返回这个对象类型的std::shared_ptr指针

    std::future<return_type> res = task->get_future(); 表示获取 std::packaged_task 对象的 std::future 对象
    std::packaged_task<return_type()> task 表示创建一个 std::packaged_task 对象
    (*task)() 表示调用 std::packaged_task 对象，即调用绑定的函数
*/
//不能放在cpp文件，原因是C++编译器不支持模版的分离编译
template<class F, class... Args> //函数模板 Args...表示可变参数模板  F表示函数模板
auto ThreadPool::add(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
    using return_type = typename std::result_of<F(Args...)>::type; 
    
    auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(tasks_mtx);

        // don't allow enqueueing after stopping the pool
        if(stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks.emplace([task](){ (*task)(); });
    }
    cv.notify_one(); //唤醒一个线程
    return res;
}
