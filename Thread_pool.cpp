#include <iostream>
#include <thread>
#include <mutex>
#include<atomic>
#include <string>
#include <memory>
#include <queue>
#include<vector>
#include <condition_variable>
#include<functional>

class ThreadPool
{
public:
    ThreadPool(int n):stop(false)    //创建n个线程，线程立即运行
    {
        for(int i=0;i<n;++i)
        {
            threads.emplace_back([this](){
                while(true)
                {
                    std::function<void()> task;    //消费者，取队列里的函数运行
                    {
                        std::unique_lock<std::mutex> lock(mtx);   //互斥访问队列tasks 
                        condition.wait(lock,[this](){
                            return !tasks.empty() || stop;
                        });
                        if(stop && tasks.empty())
                        {
                            return;
                        }
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }
    ~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(mtx);
            stop=true;
        }
        condition.notify_all();  //此时stop=true;让所有卡着的线程跳过condition.wait并且运行下一句的return 结束线程。
        for(auto& t:threads)
        {
            t.join();  //可能主进程运行完了，然而线程还未结束，在这里阻塞进程等待线程运行完。
        }
    }

    template<class F,class... Args>  //生产者
    void enqueue(F&& f,Args&&... args)
    {
        std::function<void()> task = 
            std::bind(std::forward<F>(f),std::forward<Args>(args)...);
        {
            std::unique_lock<std::mutex> lock(mtx);  //互斥访问队列tasks
            tasks.emplace(task);
        }
        condition.notify_one();  //唤醒一个阻塞的线程
    }
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex mtx;
    std::condition_variable condition;
    bool stop;   //true 表示生产完毕
};

std::mutex p_mtx;
int main()
{
    ThreadPool pool(4);
    for(int i=1;i<10;++i)
    {
        pool.enqueue([i](){
            std::unique_lock<std::mutex> lock(p_mtx);
            std::cout<<"Task:"<<i<<"dododo"<<std::endl;
            //std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout<<"Task:"<<i<<"endendend"<<std::endl;
        });
    }
    return 0;
}