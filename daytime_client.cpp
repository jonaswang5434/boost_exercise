#include <array>
#include <iostream>
#include <boost/asio.hpp>
// Asio 库用于TCP协议的命名空间
using boost::asio::ip::tcp;
int main(int argc,char* argv[])
{
    try
    {
        if (argc!=2)
        {
            std::cerr <<"Usage:Client <host>" <<std::endl;
            return 1;
        }
        boost::asio::io_context io_context;
        //ip::tcp::resolver: turn the server name that was specified as parameter to the application,into TCP endpoint.
        tcp::resolver resolver(io_context);
        // resolver.resolve: takes host name and service name,and turns them into a list of endpoints.
        //  server name is  specified in arg[1],service name is "daytime"
        tcp::resolver::results_type endpoints=resolver.resolve("localhost","daytime");
        // create and connect the socket. the list of endpoints obtained above may contain both ipv4 and ipv6 endpoints,
        // so we need to try each of them until we find one that works.
        // this keeps the client program idependent of a specific Ip version.
        // the boost::asio::connect() function does this for us automatically.
        tcp::socket socket(io_context);
        boost::asio::connect(socket,endpoints);
        // the connection is open,All we neen to do is read the response from the daytime service.
        for(;;)  // 这里的for循环，省略了初始化、条件、增量部分，相当于 while true.
        {
            // we use array to hold the recevied data.also could use a char [] or std::vector.
            std::array<char,128> buf;
            boost::system::error_code error;
            // boost::asio::buffer() automatically determains the size of the array to help prevent buffer overruns.
            size_t len=socket.read_some(boost::asio::buffer(buf),error);
            // when the server close the connection,the read_some() will exit with the boost::asio::error::eof error,which is how we know to exit the loop.
            if(error==boost::asio::error::eof)
                break;// connect closed cleanly by peer.
            else if(error)
                throw boost::system::system_error(error);

            std::cout.write(buf.data(),len);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 1;
}