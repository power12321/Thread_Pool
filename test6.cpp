// 条件变量

#include <iostream>
#include <thread>
#include <mutex>
#include<atomic>
#include <string>
#include <memory>
#include <queue>
#include <condition_variable>

std::queue<int> q;
std::condition_variable cv;
std::mutex mtx;
std::atomic<bool> done{false};  // 添加结束标志

void Producer()
{
    for (int i = 0; i < 10; i++)
    {
        {
            std::unique_lock<std::mutex> lock(mtx);
            q.push(i);
            cv.notify_one();
            std::cout << " P :" << i << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
     // 生产者结束，设置标志并通知所有消费者
    done = true;
    cv.notify_all();
}
void Consumer(int i)
{
    while (1)
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, []()
                { return !q.empty() || done; });
        // 如果队列为空且生产结束，退出循环
        if (q.empty() && done) {
            break;
        }
        int temp = q.front();
        q.pop();
        std::cout << "C" << i << ":" << temp << std::endl;
    }
}

int main()
{
    std::thread t1(Producer), t2(Consumer, 1), t3(Consumer, 2)
    ,t4(Consumer, 3), t5(Consumer, 4);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();

    return 0;
}