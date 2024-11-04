
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

class printer
{
public:
// 一个构造函数，接受一个参数，但可以初始化多个成员变量，
// 这里是参数io，但初始化了成员变量io，以及初始化成员变量count_为0，虽然count_不是构造函数的入参，但可以做出成员变量而被初始化。
// 构造函数的成员变量和入参并没有必然联系，只是入参可以被初始化为成员变量，也可以直接在构造函数内使用(不以成员变量的方式使用)
  printer(boost::asio::io_context& io)
    : timer_(io, boost::asio::chrono::seconds(1)), // timer_被初始化为io，该io拥有一个一个有效持续时间
      count_(0)  // count_被初始化为0；
  { 
    //timer_设置一个等待时间，该时间结束后调用print函数.
    // boost::bind：将一个函数和参数绑定到一起，从而创建一个新的可调用对象。
    // printer::print: 当前对象的print成员函数
    // this传入函数，为了保证在异步调用过程中，可以访问当前对象的成员函数; 而print函数需要访问类的成员变量count_和timer_,所以需要传入this
    timer_.async_wait(boost::bind(&printer::print, this));   
  }

  ~printer()
  {
    std::cout << "Final count is " << count_ << std::endl;
  }

  void print()
  {
    if (count_ < 5)
    {
      std::cout << count_ << std::endl;
      ++count_;

      timer_.expires_at(timer_.expiry() + boost::asio::chrono::seconds(1));
    //&printer::print： 是printer类的print对象的指针，this作为参数传入，以便能访问当前对象的上下文成员函数（count_和timer_)）
      timer_.async_wait(boost::bind(&printer::print, this)); 
    }
  }
// 添加私有成员，是为了确保该成员只能通过pulic方法进行修改
// 如果没有该私有成员，则两个对象可以直接在外部进行修改，降低了程序的严谨性
private:
  boost::asio::steady_timer timer_;
  int count_;
};

int main()
{
  boost::asio::io_context io;  //io_context负责管理所有异步操作的I/O服务，它维护一个事件队列，当有事件时，将这些事件放入队列中，并在合适的时间点执行这些事件。
  printer p(io); // 将io对象传入给printer的构造函数
  io.run();  // 启动io_context的事件循环

  return 0;
}