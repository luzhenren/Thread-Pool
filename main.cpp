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
        sleep(3);
        return sum;
    }
private:
    int begin_;
    int end_;
};

int main() {
    {
        ThreadPool pool;
        pool.setMode(PoolMode::MODE_CACHE);
        pool.setInitThreadSize(4);
        pool.start();

        Result res = pool.submitTask(std::make_shared<MyTask>(1, 100));
        Result res1 = pool.submitTask(std::make_shared<MyTask>(1, 100));
        Result res2 = pool.submitTask(std::make_shared<MyTask>(1, 100));
        Result res3 = pool.submitTask(std::make_shared<MyTask>(1, 100));
        Result res4 = pool.submitTask(std::make_shared<MyTask>(1, 100));
        Result res5 = pool.submitTask(std::make_shared<MyTask>(1, 100));


        int sum = res.get().cast_<int>();
        std::cout << sum << std::endl;
    }
   getchar();
}
