#ifndef THREADPOOL_THREADPOOL_H
#define THREADPOOL_THREADPOOL_H
#include <vector>
#include <memory>
#include <thread>
#include <queue>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <functional>

// 任务抽象基类
// 用户可以自定义任意类型任务，继承即可
class Task{
public:
    virtual void run() = 0;
};

// 线程池支持的模式
enum class PoolMode{
    MODE_FIXED,
    MODE_CACHE,
};

// 抽象线程类型
class Thread{
public:
    // 没有参数的函数类型
    using ThreadFunc = std::function<void()>;
    // 线程构造
    Thread(ThreadFunc func);
    // 线程析构
    ~Thread();
    void start();
private:
    ThreadFunc func_;
};

class ThreadPool {
public:
    ThreadPool();
    ~ThreadPool();

    // 设置工作模式
    void setMode(PoolMode mode);

    // 设置初始线程数量
    void setInitThreadSize(int size);

    // 设置task任务队列上限阈值
    void setTaskQueMaxThreshold(int threshold);

    // 提交任务给线程池
    void submit(std::shared_ptr<Task> sp);

    // 开启线程池
    void start();
private:
    // 定义线程函数
    void threadFunc();
private:
    std::vector<Thread*> threads_; // 线程列表
    size_t initThreadSize_; //初始线程数量
    PoolMode poolMode_;

    // 用户传入基类Task对象指针，需要拉长生命周期
    std::queue<std::shared_ptr<Task>> taskQue_; // 任务队列
    std::atomic_uint32_t taskSize_; //任务数量
    int taskSizeMaxThreshold_ ; // 任务队列阈值

    std::mutex taskQueMtx_; //保证任务队列线程安全
    std::condition_variable notFull_; // 任务队列不满
    std::condition_variable notEmpty_;
};


#endif //THREADPOOL_THREADPOOL_H
