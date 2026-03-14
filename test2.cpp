#include<iostream>
#include<thread>
#include<string>

void print(int& s)
{
    s+=1;
}
int main()
{
    int s=1;
    std::thread t1(print,std::ref(s));
    if(t1.joinable())
    {
        t1.join();
    }
    std::cout<<s<<std::endl;
    return 0;
}