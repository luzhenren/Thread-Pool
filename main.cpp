#include <iostream>
#include <unistd.h>
#include "./include/threadpool.h"

class MyTask : public Task {
public:
    void run() {
        std::cout << "tid" << std::this_thread::get_id() << "begin!" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "tid" << std::this_thread::get_id() << "end!" << std::endl;
    }
};

int main() {
    ThreadPool pool;
    pool.setInitThreadSize(4);
    pool.start();

    pool.submitTask(std::make_shared<MyTask>());
    pool.submitTask(std::make_shared<MyTask>());
    pool.submitTask(std::make_shared<MyTask>());
    pool.submitTask(std::make_shared<MyTask>());
    pool.submitTask(std::make_shared<MyTask>());
    pool.submitTask(std::make_shared<MyTask>());
    pool.submitTask(std::make_shared<MyTask>());
    pool.submitTask(std::make_shared<MyTask>());

   getchar();
}
