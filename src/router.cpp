#include "router.hpp"
#include "post_storage.hpp"
#include "DataBase_storage.hpp"


// 路由主入口函数，处理不同的请求路径和方法
http::response<http::string_body> Router::route_request(const http::request<http::string_body>& req) {
    std::string target_path = std::string(req.target()); // 转换为 std::string
    if (req.method() == http::verb::get && target_path == "/") {
        return render_homepage();
    } else if (req.method() == http::verb::get && target_path == "/posts") {
        return render_posts();
    } else if (req.method() == http::verb::get && target_path.rfind("/posts/", 0) == 0) {
        // 解析ID，确保 "/posts/{id}" 路径被处理
        int post_id = std::stoi(target_path.substr(7));  // 提取ID
        return render_post(post_id);
    } else if (req.method() == http::verb::get && target_path == "/new-post") {
        return render_new_post_form();
    } else if (req.method() == http::verb::post && target_path == "/new-post") {
        return handle_new_post(req);
    }

    // 如果路径不匹配，返回404
    http::response<http::string_body> res;
    res.result(http::status::not_found);
    res.set(http::field::content_type, "text/html");
    res.body() = "<html><body><h1>404 Not Found</h1></body></html>";
    res.prepare_payload();
    return res;
}

http::response<http::string_body> Router::render_homepage() {
    std::string html_content = HtmlHandle::load_html_template("resources/index.html");

    http::response<http::string_body> res;
    res.result(http::status::ok);
    res.set(http::field::content_type, "text/html");
    res.body() = html_content;
    res.prepare_payload();
    return res;
}

http::response<http::string_body> Router::render_posts() {
    std::string html_content = HtmlHandle::load_html_template("resources/posts.html");

    // 获取所有文章
    auto posts = PostStorage::get_instance().get_all_posts();
    std::stringstream posts_list;

    //根据时间排序
    std::sort(posts.begin(), posts.end(), [](const BlogPost& a, const BlogPost& b) {
        return a.created_at > b.created_at;
    });

    // 动态生成文章列表与作者姓名的HTML
    for (const auto& post : posts) {
        posts_list << "<li>"
                   << "<a href='/posts/" << post.id << "'>" << post.title << "</a>"
                   << "<p style='font-size: 0.9em; color: #666;'>作者: " << post.author << "</p>"  // 显示作者信息
                    <<"<p style='font-size: 0.5em; color: #233;'>发布时间: "<<post.created_at<<"</p>"   // 显示创建时间
                   << "</li>";
    }

    HtmlHandle::replace_placeholder(html_content, "{{posts_list}}", posts_list.str());

    // 打印调试信息
    //std::cout << "Generated HTML:\n" << html_content << std::endl;

    http::response<http::string_body> res;
    res.result(http::status::ok);
    res.set(http::field::content_type, "text/html");
    res.body() = html_content;
    res.prepare_payload();
    return res;
}

http::response<http::string_body> Router::render_post(int id) {
    std::string html_content = HtmlHandle::load_html_template("resources/post.html");
    auto post = PostStorage::get_instance().get_post_by_id(id);

    HtmlHandle::replace_placeholder(html_content, "{{post_title}}", post.title);
    HtmlHandle::replace_placeholder(html_content, "{{post_content}}", post.content);
    HtmlHandle::replace_placeholder(html_content, "{{post_author}}", post.author);  // 替换作者
    HtmlHandle::replace_placeholder(html_content, "{{post_time}}", post.created_at);

    // 调试信息：检查生成的HTML
    //std::cout << "Generated HTML:\n" << html_content << std::endl;

    http::response<http::string_body> res;
    res.result(http::status::ok);
    res.set(http::field::content_type, "text/html");
    res.body() = html_content;
    res.prepare_payload();
    return res;
}



http::response<http::string_body> Router::render_new_post_form() {
    std::string html_content = HtmlHandle::load_html_template("resources/new_post_form.html");

    http::response<http::string_body> res;
    res.result(http::status::ok);
    res.set(http::field::content_type, "text/html");
    res.body() = html_content;
    res.prepare_payload();
    return res;
}

// 处理表单提交的函数
http::response<http::string_body> Router::handle_new_post(const http::request<http::string_body>& req) {
    auto body = req.body();

    // 提取作者、标题和内容，确保截取范围正确
    size_t author_start = body.find("author=") + 7;
    size_t title_start = body.find("title=") + 6;
    size_t content_start = body.find("content=") + 8;

    size_t author_end = body.find("&", author_start);
    size_t title_end = body.find("&", title_start);
    size_t content_end = body.length();

    // 防止未找到 '&' 的情况下导致的错误
    if (author_end == std::string::npos) {
        author_end = title_start - 6; // 如果没有 '&'，说明 author 是单独存在的
    }

    if (title_end == std::string::npos) {
        title_end = content_start - 8; // 如果没有 '&'，说明 title 是单独存在的
    }

    std::string author = body.substr(author_start, author_end - author_start);
    std::string title = body.substr(title_start, title_end - title_start);
    std::string content = body.substr(content_start, content_end - content_start);


    // 使用 URL 解码
    author = HtmlHandle::url_decode(author);
    title = HtmlHandle::url_decode(title);
    content = HtmlHandle::url_decode(content);

    // 初始化禁止关键词
    Forbidden::init_forbidden_keywords();
    // 过滤用户的提交内容
    author = Forbidden::filter_content(author);
    title = Forbidden::filter_content(title);
    content = Forbidden::filter_content(content);

    // 打印调试信息
    std::cout << "Content Debug: " << content << std::endl;

    // 清除末尾可能多余的非可见字符或空白字符
    author.erase(std::find_if(author.rbegin(), author.rend(), [](unsigned char ch) {
                     return !std::isspace(ch);
                 }).base(), author.end());

    title.erase(std::find_if(title.rbegin(), title.rend(), [](unsigned char ch) {
                    return !std::isspace(ch);
                }).base(), title.end());

    content.erase(std::find_if(content.rbegin(), content.rend(), [](unsigned char ch) {
                      return !std::isspace(ch);
                  }).base(), content.end());


    // 打印调试信息
    /*
    std::cout << "Author: " << author << std::endl;
    std::cout << "Title: " << title << std::endl;
    std::cout << "Content: " << content << std::endl;
    */
    //获取北京时间
    std::time_t now = std::time(nullptr);
    std::string time = std::asctime(std::gmtime(&now));
    time.erase(std::remove(time.begin(), time.end(), '\n'), time.end());


    // 将新文章添加到存储中
    PostStorage::get_instance().add_post(title, content, author,time);

    // 将文章存储到SQLITE数据库
    DataBase_storage::getInstance().add_post(title, content, author,time);

    http::response<http::string_body> res;
    res.result(http::status::found);
    res.set(http::field::location, "/posts");  // 重定向到文章列表页
    return res;
}
