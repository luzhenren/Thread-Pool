//
// Created by 19393 on 2025/1/3.
//

#include "../include/threadpool.h"
#include <thread>
#include <iostream>

const int TASK_MAX_THREADSHOLD = 1024;

ThreadPool::ThreadPool()
    : initThreadSize_(4),
    taskSize_(0),
    taskSizeMaxThreshold_(TASK_MAX_THREADSHOLD),
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

void ThreadPool::submit(std::shared_ptr<Task> sp) {

}
void ThreadPool::start() {
    // 创建线程对象
    for (int i = 0; i < initThreadSize_; i++) {
        // 创建thread线程对象时，把线程函数绑定到线程对象上
        threads_.emplace_back(new Thread(std::bind(&ThreadPool::threadFunc, this)));
    }

    // 启动所有线程
    for (int i = 0; i < initThreadSize_; i++) {
        threads_[i]->start(); // 执行线程池里的函数
    }
}

void ThreadPool::threadFunc() {
    std::cout << "begin threadFunc" << std::endl;
    std::cout << std::this_thread::get_id() << std::endl;
    std::cout << "end threadFunc" << std::endl;
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




