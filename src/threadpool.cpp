//
// Created by 19393 on 2025/1/3.
//

#include "../include/threadpool.h"
#include <thread>
#include <iostream>
#include <unistd.h>

const int TASK_MAX_THRESHOLD = 1024;

ThreadPool::ThreadPool()
    : initThreadSize_(4),
    taskSize_(0),
    taskSizeMaxThreshold_(TASK_MAX_THRESHOLD),
    poolMode_(PoolMode::MODE_FIXED) {

}

ThreadPool::~ThreadPool() {

}

void ThreadPool::setMode(PoolMode mode) {
    poolMode_ = mode;
}

void ThreadPool::setInitThreadSize(int size) {
    initThreadSize_ = size;
}

void ThreadPool::setTaskQueMaxThreshold(int threshold) {
    taskSizeMaxThreshold_ = threshold;
}

void ThreadPool::submitTask(std::shared_ptr<Task> sp) {
    // 获取锁
    std::unique_lock<std::mutex> lock(taskQueMtx_);
    // 线程通信等待任务队列有空余
    if (!notFull_.wait_for(lock, std::chrono::seconds(1) ,
                           [&]()->bool {return taskQue_.size() < taskSizeMaxThreshold_;})) {
        std::cerr << "task queue is full , submit task failed" << std::endl;
        return ;
    }
    taskQue_.emplace(sp);
    taskSize_++;

    // 任务队列不空了，在notEmpty通知·
    notEmpty_.notify_all();
}
void ThreadPool::start() {
    // 创建线程对象
    for (int i = 0; i < initThreadSize_; i++) {
        // 创建thread线程对象时，把线程函数绑定到线程对象上
        std::unique_ptr<Thread> ptr = std::make_unique<Thread>(std::bind(&ThreadPool::threadFunc, this));
        threads_.emplace_back(std::move(ptr));
    }

    // 启动所有线程
    for (int i = 0; i < initThreadSize_; i++) {
        threads_[i]->start(); // 执行线程池里的函数
    }
}

void ThreadPool::threadFunc() {
//    std::cout << "begin threadFunc tid" << std::this_thread::get_id() << std::endl;
//    sleep(2);
//    std::cout << "end threadFunc tid" << std::this_thread::get_id() << std::endl;
    for(;;) {
        std::shared_ptr<Task> task;
        {
            // 先获取锁
            std::unique_lock<std::mutex> lock(taskQueMtx_);
            std::cout << "tid: " << std::this_thread::get_id() << "try to get task" << std::endl;
            // 等待notEmpty
            notEmpty_.wait(lock, [&]()->bool {return !taskQue_.empty();});
            std::cout << "tid: " << std::this_thread::get_id() << " get task success" << std::endl;
            // 取任务
            task = taskQue_.front();
            taskQue_.pop();
            taskSize_--;

            // 如果依然有剩余任务，通知线程执行
            if (!taskQue_.empty()) {
                notEmpty_.notify_all();
            }
            // 通知继续生产任务
            notFull_.notify_all();
        }
        // 执行任务
        if (task != nullptr) {
            task->run();
        }
    }
}

///////////////////////// 线程方法实现
Thread::Thread(ThreadFunc func):func_(func) {

}

Thread::~Thread() {

}

// 启动线程
void Thread::start() {
    // 线程需要执行的函数，应该由线程池提供，所以需要使用bind
    std::thread t(func_);
    t.detach();
}




