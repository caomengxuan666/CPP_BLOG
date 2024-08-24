#include "server.hpp"
#include <boost/asio/io_context.hpp>
#include <iostream>
#include "DataBase_storage.hpp"

const int port=8085;

int main() {
    //加载数据库
    DataBase_storage::getInstance().load_posts_from_dataBase();
    try {
        boost::asio::io_context ioc;
        tcp::endpoint endpoint(tcp::v4(), port);

        std::make_shared<Server>(ioc, endpoint)->run();
        std::cout<<"服务器已经开始运行..."<<"运行地址"<<endpoint<<std::endl;

        ioc.run();// 事件循环，异步处理
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
