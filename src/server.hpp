#ifndef SERVER_HPP
#define SERVER_HPP

#include <boost/asio.hpp>
#include <memory>
#include <vector>
#include <iostream>
#include "session.hpp"

using tcp = boost::asio::ip::tcp;

class Server : public std::enable_shared_from_this<Server> {
public:
    Server(boost::asio::io_context& ioc, tcp::endpoint endpoint);

    void run();

private:
    void do_accept();

    tcp::acceptor acceptor_;
    boost::asio::io_context& ioc_;
};

#endif
