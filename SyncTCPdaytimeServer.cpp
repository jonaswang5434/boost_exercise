#include<ctime>
#include<iostream>
#include<string>
#include<boost/asio.hpp>
using boost::asio::ip::tcp;

std::string make_daytime_string()
{
    using namespace std;
    time_t now=time(0);
    return ctime(&now);    
}

int main()
{
    try
    {
        boost::asio::io_context io_context;
        // tcp::acceptor object needs to be created to listen for new connections.It is initialised on TCP port 13,for ip version 4.
        tcp::acceptor acceptor (io_context,tcp::endpoint(tcp::v4(),13));
        for(;;)
        {
            tcp::socket socket(io_context); // create a TCP socket using the provided io_context
            // acceptor.accept: accepts a new incoming connection and associates it with the socket.
            // this means that once a client connect, the socket can be used to communicate with that client.
            acceptor.accept(socket);
            std::string message=make_daytime_string();
            boost::system::error_code ignored_error; // used to capture any errors that occur during the write operation
            // send the message stored in message over the socket.
            // asio::buffer(): creates a buffer from the string to be sent.
            // ignored_error: used to capture any errors that occur during the write operation, but since it is not checked afterward,
            // any potential errors will not be handled.
            boost::asio::write(socket,boost::asio::buffer(message),ignored_error);
        }
    }
    catch(std::exception & e)
    {
        std::cerr<<e.what()<<std::endl;
    }
    return 0;
    
}