#pragma once

#include <algorithm>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

class ThreadPool {
    using func = std::function<void(void)>;
    using pool = std::vector<std::thread>;
private:
    unsigned int _num_threads;
    pool _threads;

    std::mutex _lock;
    std::condition_variable _cv;

    std::queue<func> _work;

    bool _stopped;
public:

    ThreadPool() : ThreadPool(4) { }
    ThreadPool(unsigned int num_threads) :
        _num_threads(std::min(num_threads, std::thread::hardware_concurrency())),
        _stopped(false) {
        for (int i = 0; i < _num_threads; i++) {
            _threads.emplace_back(std::thread([this]{
                    while(true) {
                        func f;
                        {
                            std::unique_lock<std::mutex> unique_lock(_lock);

                            _cv.wait(unique_lock, [this](){ return _stopped || !_work.empty(); });
                            if (_stopped && _work.empty()) { return; }

                             f = std::move(_work.front());
                            _work.pop();
                        }

                        f();
                    }
                }));
        }
    }

    template<typename F, typename... Args>
    void push(F&& f, Args&&... args) {
        {
            std::unique_lock<std::mutex> unique_lock(_lock);
            _work.emplace([=](){ f(args...); });
            _cv.notify_one();
        }

    }

    void stop() {
        _stopped = true;
        _cv.notify_all();

        for (int i = 0; i < _num_threads; i++) {
            _threads[i].join();
        }
    }
};