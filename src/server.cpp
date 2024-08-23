#include "server.hpp"

Server::Server(boost::asio::io_context& ioc, tcp::endpoint endpoint)
    : ioc_(ioc), acceptor_(ioc, endpoint) {}

void Server::run() {
    do_accept();
}

void Server::do_accept() {
    acceptor_.async_accept(
            [self = shared_from_this()](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::make_shared<Session>(std::move(socket))->start();
                }
                self->do_accept();  // 继续接受下一个连接
            });
}
