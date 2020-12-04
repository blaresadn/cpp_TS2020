#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <future>

class ThreadPool
{
private:
    std::vector<std::thread> pool_;
    std::queue<std::function<void()> > tasks_;
    std::mutex mutex_;
    std::condition_variable dataReady_;
    bool f = false;

public:
    ThreadPool(size_t poolSize)
    {
        if (poolSize <= 0) {
            throw std::runtime_error("Incorrect pool size");
        }
        for(size_t i = 0; i < poolSize; i++) {
            pool_.emplace_back([this]
            {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(mutex_);
                        while (tasks_.empty() && !f) {
                            dataReady_.wait(lock);
                        }
                        if (tasks_.empty()) {
                            break;
                        }
                        task = std::move(tasks_.front());
                        tasks_.pop();
                    }
                    task();
                }
            });
        }
    }

    template <class Func, class... Args>
    auto exec(Func func, Args... args) -> std::future<decltype(func(args...))>
    {
        auto task = std::make_shared<std::packaged_task<decltype(func(args...))()> >
                (std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
        auto res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(mutex_);
            tasks_.emplace([task]() {(*task)();});
        }
        dataReady_.notify_one();
        return res;
    }

    ~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            f = true;
        }
        dataReady_.notify_all();
        for (std::thread &th: pool_) {
            th.join();
        }
    }
};
