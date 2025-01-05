#include <iostream>
#include <unistd.h>
#include "./include/threadpool.h"

class MyTask : public Task {
public:
    MyTask():begin_(0), end_(0){}
    ~MyTask(){}
    MyTask(int b, int e):begin_(b), end_(e){}

    Any run() {
        int sum = begin_ + end_;
        return sum;
    }
private:
    int begin_;
    int end_;
};

int main() {
    ThreadPool pool;
    pool.setInitThreadSize(4);
    pool.start();

    Result res = pool.submitTask(std::make_shared<MyTask>(1, 100));

    int sum = res.get().cast_<int>();

    std::cout << sum << std::endl;
//    pool.submitTask(std::make_shared<MyTask>());
//    pool.submitTask(std::make_shared<MyTask>());
//    pool.submitTask(std::make_shared<MyTask>());
//    pool.submitTask(std::make_shared<MyTask>());
//    pool.submitTask(std::make_shared<MyTask>());
//    pool.submitTask(std::make_shared<MyTask>());
//    pool.submitTask(std::make_shared<MyTask>());
//    pool.submitTask(std::make_shared<MyTask>());

   getchar();
}
