#include <functional>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>

class printer
{
public:
  printer(boost::asio::io_context& io)
    : strand_(boost::asio::make_strand(io)),
      timer1_(io, boost::asio::chrono::seconds(1)),
      timer2_(io, boost::asio::chrono::seconds(1)),
      count_(0)
  {
    timer1_.async_wait(boost::asio::bind_executor(strand_,
          std::bind(&printer::print1, this)));

    timer2_.async_wait(boost::asio::bind_executor(strand_,
          std::bind(&printer::print2, this)));
  }

  ~printer()
  {
    std::cout << "Final count is " << count_ << std::endl;
  }

  void print1()
  {
    if (count_ < 10)
    {
      std::cout << "Timer 1: " << count_ << std::endl;
      ++count_;

      timer1_.expires_at(timer1_.expiry() + boost::asio::chrono::seconds(1));

      timer1_.async_wait(boost::asio::bind_executor(strand_,    //这里的boost::asio::bind_executor 是避免资源竞争的关键，这确保通过它提交的任务在同一时间，只能有一个线程执行。
            std::bind(&printer::print1, this)));
    }
  }

  void print2()
  {
    if (count_ < 10)
    {
      std::cout << "Timer 2: " << count_ << std::endl;
      ++count_;

      timer2_.expires_at(timer2_.expiry() + boost::asio::chrono::seconds(1));

      timer2_.async_wait(boost::asio::bind_executor(strand_,  // strand_是boost::asio::strand对象，它确保提交的函数不会被并发执行。bind_executor:在指定的执行器上下文执行回调函数。
            std::bind(&printer::print2, this)));//&printer是指针::print2是成员函数， bind()将this作为第一个参数传给成员函数，成员函数不能单独调用，需要this指针
    }
  }

private:
  boost::asio::strand<boost::asio::io_context::executor_type> strand_; // boost::asio::strand 是模板类，参数为executor类型，strand_是一个与特定io_context关联的实例
  boost::asio::steady_timer timer1_;
  boost::asio::steady_timer timer2_;
  int count_;
};

int main()
{
  boost::asio::io_context io;
  printer p(io);
  std::thread t([&]{ io.run(); });  // lambda表达式，[&] 指任何lambda中的变量都以引用使用，{io.run()}是函数体

  std::cout<<"子线程在执行中1,不阻塞此代码执行" << std::endl;
  io.run(); //This is where the event loop starts running
  t.join(); // 等带子线程执行结束。

  return 0;
}