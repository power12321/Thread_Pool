#include<iostream>
#include<thread>
#include<string>

void print(std::string s)
{
    int n=10;
    while(n--)std::cout<<s<<std::endl;
}
int main()
{
    std::thread t1(print,"sssss");
    // if(t1.joinable())
    // {
    //     t1.join();
    // }
    t1.detach();
    std::cout<<"aaaaaaaaaaaaaaaa"<<std::endl;
    return 0;
}