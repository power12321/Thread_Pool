#include<iostream>
#include<thread>
#include<string>
#include<memory>

class A
{
private:
    friend void f();
    void print()
    {
        std::cout<< "sssssssssss" << std::endl;
    }
};

void f()
{
    std::shared_ptr<A> ptr=std::make_shared<A>();

    std::thread t1(&A::print,ptr);

    t1.detach();
    //t1.join();
}
int main()
{
    f();
    std::cout<<"last  "<<std::endl;
    return 0;
}