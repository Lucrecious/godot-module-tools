#include "threadpool.hpp"

#include <chrono>
#include <iostream>
#include <thread>

int main(int cargs, char** args) {

    {
        std::cout << "No work stop test" << std::endl;
        ThreadPool pool(4);
        pool.stop();
    }

    {
        std::cout<< "Work is limited by threads test" << std::endl;
        ThreadPool pool(2);

    
        auto start_test2 = std::chrono::system_clock::now();

        std::mutex lk;

        for (int i = 0; i < 10; i++) {
            pool.push([start_test2, &lk](){
                std::this_thread::sleep_for(std::chrono::seconds(1));
                {
                    std::unique_lock<std::mutex> unique_lk(lk);
                    std::cout << (std::chrono::system_clock::now() - start_test2).count() << " seconds have passed." << std::endl;
                }
            });
        }

        std::this_thread::sleep_for(std::chrono::seconds(5));

        pool.stop();
    }

    return 0;
}