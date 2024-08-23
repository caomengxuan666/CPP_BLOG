#ifndef ROUTER_HPP
#define ROUTER_HPP

#include <boost/beast.hpp>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>
#include "HtmlHandle.hpp"

namespace beast = boost::beast;
namespace http = beast::http;

class Router {
public:
    // 处理路由的主入口函数
    http::response<http::string_body> route_request(const http::request<http::string_body>& req);

private:
    // 各种页面的渲染函数声明

    /**
     * @brief 主页页面
     * @return 主页页面的响应，下面都是一样的
     * @note 渲染主页页面，返回主页页面的响应，下面基本都是一样的
     * @author 曹梦轩
     */
    http::response<http::string_body> render_homepage();

    /**
     * @brief 所有的posts帖子的显示界面
     */
    http::response<http::string_body> render_posts();

    /**
     * @brief 具体的帖子的界面
     * @param id 具体帖子界面的id
     */
    http::response<http::string_body> render_post(int id);

    /**
     * @brief 提交帖子的界面
     */

    http::response<http::string_body> render_new_post_form();


    /**
     * @brief 处理表单提交的函数
     * @param request，实际上使用的只有它的body
     */
    http::response<http::string_body> handle_new_post(const http::request<http::string_body>& req);
};

#endif
