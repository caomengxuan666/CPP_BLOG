#include "session.hpp"
#include "router.hpp"


Session::Session(tcp::socket socket)
    : socket_(std::move(socket)) {}

void Session::start() {
    read_request();
}

void Session::read_request() {
    auto self = shared_from_this();
    http::async_read(socket_, buffer_, req_,
                     [self](beast::error_code ec, std::size_t bytes_transferred) {
                         boost::ignore_unused(bytes_transferred);
                         if (!ec) {
                             self->process_request();
                         }
                     });
}

void Session::process_request() {
    Router router;
    res_ = router.route_request(req_);  // 路由模块处理请求
    write_response();
}

void Session::write_response() {
    auto self = shared_from_this();

    // 获取响应内容
    std::string body_content = res_.body();

    // 更新处理后的内容
    res_.body() = body_content;
    res_.prepare_payload();  // 重新准备响应的内容和头部信息

    // 打印调试信息
    //std::cout << "Processed HTML Response:\n" << res_.body() << std::endl;

    http::async_write(socket_, res_,
                      [self](beast::error_code ec, std::size_t bytes_transferred) {
                          boost::ignore_unused(bytes_transferred);
                          if (!ec) {
                              self->socket_.shutdown(tcp::socket::shutdown_send, ec);
                          }
                      });
}
