#include <thread>
#include <chrono>
#include <iostream>
#include <mutex>
#include <vector>
#include <condition_variable>
#include <future>
#include <queue>



// template<typename F, typename R, typename ...Args>
// return type of ret takes no arguments
template<typename ret>
class Thread_pool{
    using arg = void;
    using Task = std::function<ret(arg)>;

    public:
    Thread_pool(size_t n){
        init_thread_v(n);
        // std::cout<< "threadpool initialized"<< std::endl;
    }

    Thread_pool(){
        size_t n = std::thread::hardware_concurrency();
        init_thread_v(n);
        // std::cout<< "threadpool initialized"<< std::endl;
    }

    ~Thread_pool(){
        destroy();
        // std::cout<< "threadpool destroyed"<< std::endl;
    }

    std::future<ret> enqueue(Task t)
    {
        std::packaged_task<typename std::result_of<Task()>::type (arg)> pt(t);
        std::future<ret> f = pt.get_future();
        {
            std::unique_lock lk(m);
            // enqueue by moving
            q.emplace(std::move(pt));
        }
        cv.notify_one();
        return f;
    }

    private:


    // initialize n threads witha a lambda function
    // this lambda function has a whiletrue loop, a conditional variable, tha ability to pop the queue and run a task.
    void init_thread_v(size_t n){
        for (size_t i = 0 ; i < n; i++){
            thread_v.push_back(std::thread([&](){
                    while (true){
                        // define tha package task
                        std::packaged_task<ret(arg)> f;
                        {
                            // lock thread queue
                            std::unique_lock lk(m);
                            // wait for notifications
                            cv.wait(lk,[&]{ 
                                // conditonal variable test
                                return (!q.empty() | terminate);
                                });

                            if (terminate){
                                // break teh loop when the bool terminate is true
                                break;
                            }
                            // pop the queue
                            f = std::move(q.front());
                            q.pop();
                        }
                        // execute work outside of the lock
                        f();
                    }
                }
            ));
        }
    }

    void destroy(){
        {
            std::unique_lock lk(m);
            terminate = true;
        }
        cv.notify_all();
        // wait to join all thread in the thread_vector
        for (auto &thread: thread_v){
            thread.join();
        }
    }

    std::vector<std::thread> thread_v;
    std::mutex m;
    std::condition_variable cv;
    std::queue< std::packaged_task<ret(arg)> > q;
    bool terminate = false;
};