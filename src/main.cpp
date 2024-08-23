#include "server.hpp"
#include <boost/asio/io_context.hpp>
#include <iostream>


int main() {
    try {
        boost::asio::io_context ioc;
        tcp::endpoint endpoint(tcp::v4(), 8085);

        std::make_shared<Server>(ioc, endpoint)->run();

        ioc.run();  // 事件循环，异步处理
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
