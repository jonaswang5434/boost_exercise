#include <iostream>
#include <boost/asio.hpp>

void print(const boost::system::error_code& /*e*/)
{
    std::cout<<" ==========hello world! ========="<<std::endl;
}
int main()
{
    //all programs use asio need at least on I/0 execution context,eg:io_context/thread_pool
    // io execution context provides asscess to I/O functionality
    boost::asio::io_context io;  
    //the core asio class provide I/O functionality (eg:stready_timer) always take an executor as their first constructor argument.
    // the second argument to constructor sets the timer to expire 5 minutes  from now.
    boost::asio::steady_timer t(io,boost::asio::chrono::seconds(5));
    t.async_wait(&print); // 异步等待，会继续执行cout<<"complished"<<
    std::cout<<"complished "<<std::endl;
    io.run();   // 阻塞当前线程，直到io_context中的所有任务完成
    
    return 0;
}


