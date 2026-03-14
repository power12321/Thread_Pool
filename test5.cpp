#include<iostream>
#include<thread>
#include<mutex>
#include<string>
#include<memory>
class Log;
static std::unique_ptr<Log> p=nullptr;
static std::once_flag once;

class Log
{
public:
    Log(const Log& log) = delete;
    Log& operator=(const Log& log) = delete;

    static Log& get_log()
    {
        std::call_once(once,init);

        return *p;
    }

    static void init()
    {
        if(!p)
        {
            p.reset(new Log);
        }
    }

    void print(std::string s)
    {
        std::cout<<s<<std::endl;
    }
private:
    Log(){}
};

void p_error()
{
    Log::get_log().print("error");
}
int main()
{
    std::thread t1(p_error);
    std::thread t2(p_error);
    t1.join();
    t2.join();
    return 0;
}