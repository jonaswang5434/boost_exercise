#include <functional>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>

class printer
{
public:
  // 初始化strand_成员,stand class template是executor adapter,guarantees those handlers are dispatched through it.
  printer(boost::asio::io_context& io)
    : strand_(boost::asio::make_strand(io)), 
      timer1_(io, boost::asio::chrono::seconds(1)),
      timer2_(io, boost::asio::chrono::seconds(1)),
      count_(0)
  {
    // bind_executor:used to bind a function to a sepcific execution context,which is provied by strand.
    // strand: ensures the handlers that are posted to it are executed sequentially,providing a way to protect shared resource from concurrent access.
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

      timer1_.async_wait(boost::asio::bind_executor(strand_,
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

      timer2_.async_wait(boost::asio::bind_executor(strand_,
            std::bind(&printer::print2, this)));
    }
  }

private:
  boost::asio::strand<boost::asio::io_context::executor_type> strand_;
  boost::asio::steady_timer timer1_;
  boost::asio::steady_timer timer2_;
  int count_;
};

int main()
{
  boost::asio::io_context io;
  printer p(io);
  // By running io.run() in a separate thread, the main thread can continue executing other code  while the asynchronous operations managed by Boost.
  std::thread t([&]{ io.run(); });  // t:the name of thread object.
  // []:a lambda capture clause,indicates that what variables from the surrounding scope are accessible inside the lambda function. 
  // &: means the lambda will capture all variable by reference.this allow the lambda to access and modify variables from the outer scope.
  // io.run: this is the body of the lambda function,when the thread starts.

  io.run();
  t.join();

  return 0;
}