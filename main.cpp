#include <iostream>
#include <unistd.h>
#include "./include/threadpool.h"

int main() {
    ThreadPool pool;
    pool.start();
    while(true){
        sleep(2);
    }
}
