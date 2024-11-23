#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

class tcp_connection
: public boost::enable_shared_from_this<tcp_connection>      //为了可以使用共享指针 shared_ptr ;允许类对象在其成员函数中通过 shared_from_this() 来获得指向自身的 shared_ptr
{
public:
  typedef boost::shared_ptr<tcp_connection> pointer; // 声明一个tcp_connection类型的共享指针，在多处使用

  static pointer create(boost::asio::io_context& io_context)
  {
    return pointer(new tcp_connection(io_context));
  }

  tcp::socket& socket()    // 一个成员函数，返回私有变量socket_
  {
    return socket_;
  }

  void start()
  {
    message_ = make_daytime_string();

    boost::asio::async_write(socket_, boost::asio::buffer(message_), //async_write:将数据从buffer写入socker_
        //shared_from_this():来自于boost::enable_shared_from_this，当继承该类时，即可使用该函数，确保返回的share_pointer和原始的share_pointer共享引用同一个对象
        boost::bind(&tcp_connection::handle_write, shared_from_this(),   //shared_from_this，传入当前对象的指针，
          boost::asio::placeholders::error,                   // 占位符，表示操作完成后的错误信息
          boost::asio::placeholders::bytes_transferred)); //bytes_transferred:表示写入的字节数
  }

private:
  tcp_connection(boost::asio::io_context& io_context)
    : socket_(io_context)
  {
  }

  void handle_write(const boost::system::error_code& /*error*/,
      size_t /*bytes_transferred*/)
  {
  }

  tcp::socket socket_;
  std::string message_;
};

class tcp_server
{
public:
  tcp_server(boost::asio::io_context& io_context)
    : io_context_(io_context),
      acceptor_(io_context, tcp::endpoint(tcp::v4(), 13))
  {
    start_accept();
  }

private:
  void start_accept()
  {
    tcp_connection::pointer new_connection =
      tcp_connection::create(io_context_);

    acceptor_.async_accept(new_connection->socket(), // 当新连接到达时，会接受这个新连接，然后调用后面的bind绑定的函数进行处理
        boost::bind(&tcp_server::handle_accept, this, new_connection, // this 表示绑定到当前对象实例上
          boost::asio::placeholders::error));
  }

  void handle_accept(tcp_connection::pointer new_connection,
      const boost::system::error_code& error)
  {
    if (!error)
    {
      new_connection->start();
    }

    start_accept();
  }

  boost::asio::io_context& io_context_;
  tcp::acceptor acceptor_;
};

int main()
{
  try
  {
    boost::asio::io_context io_context;
    tcp_server server(io_context);
    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}