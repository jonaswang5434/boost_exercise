#include<array>
#include<iostream>
#include<boost/asio.hpp>

using boost::asio::ip::udp;
int main(int argc,char* argv[])
{
    try 
    {
        if (argc!=2)
        {std::cerr<<"usage:Client <host>" <<std::endl;
        return 1;
        }
        boost::asio::io_context io_context;
        // udp::resolver: find the correct remote endpoint to use based on the host and service names.
        // The query is restricted to return only IPV4 endpoints by the ip::udp::v4() argument.
        udp::resolver resolver(io_context);
        // udp::resolver::resolve(): guaranteed to return at least one endpoint in the list if it does not fail.
        // this means it's safe to dereference the return value directly.
        // udp::endpoint: 表示UDP端点的类，包含了ip和端口，用以存储解析后的接收端信息。
        // resolve():将service name and port 解析成一个或多个endpoint对象,返回一个迭代器范围，指向解析所有endpoint对象，
        // begin():指向第一个解析结果的迭代器。
        // *：解引用，以获取第一个udp::endpoint对象。
        udp::endpoint receiver_endpoint=*resolver.resolve(udp::v4(),argv[1],"daytime").begin();
        // since UDP is datagram-oriented,we will not be using a stream socket.Create an ip::udp::socket and initiate contact with the remote endpoint.
        udp::socket socket(io_context);
        socket.open(udp::v4()); //open the socket for communication.
        std::array<char,1> send_buf={{0}};
        // send data to a specified remoted endpoint
        socket.send_to(boost::asio::buffer(send_buf),receiver_endpoint);

        std::array<char,128> recv_buf;
        udp::endpoint sender_endpoint;
        // the endpoint on our side that receives the server's reponse will be initialised by udp::socket::receive_from()
        // receive_from():used to receive data from remote sender,return the number of bytes received.
        size_t len=socket.receive_from(
            boost::asio::buffer(recv_buf),sender_endpoint);  //recv_buf:will hold the received data. and sender_endpoint will be populated with the address and port of the sender.
        std::cout.write(recv_buf.data(),len); // write raw data to the standard output.
    }
    catch (std::exception & e)
    {
        std::cerr <<e.what() <<std::endl;
    }
    return 0;
}