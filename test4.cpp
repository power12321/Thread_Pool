#include<iostream>
#include<thread>
#include<string>
#include<mutex>

std::mutex mtx;
int a=0;

void f()
{
    // for(int i=0;i<100000000;++i)
    // {
    //     mtx.lock();
    //     a+=1;
    //     mtx.unlock();
    // }
    int local=0;
    for(int i=0;i<100000000;++i)
    {
        local+=1;
    }
    mtx.lock();
    a+=local;
    mtx.unlock();
}
int main()
{
    std::thread t1(f);
    
    std::thread t2(f);
    t1.join();
    t2.join();

    std::cout<<a<<std::endl;
    return 0;
}