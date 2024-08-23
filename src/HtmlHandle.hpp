//
// Created by CaoMengxuan on 2024/8/23.
//

#ifndef BLOG_HTMLHANDLE_HPP
#define BLOG_HTMLHANDLE_HPP

#include <iostream>
#include <sstream>
#include <fstream>


class HtmlHandle {
public:
    // 辅助函数：读取HTML文件内容
    static std::string load_html_template(const std::string& file_path) {
        auto path="../"+file_path;
        std::ifstream file(path);
        if (!file) {
            throw std::runtime_error("Could not open file: " + file_path);
        }

        std::stringstream buffer;
        buffer << file.rdbuf(); // 读取文件内容到缓冲区
        return buffer.str();
    }

    // HTML转义函数，确保特殊字符被正确转义
    static std::string escape_html(const std::string& data) {
        std::string buffer;
        buffer.reserve(data.size());
        for (char ch : data) {
            switch (ch) {
                case '&': buffer.append("&amp;"); break;
                case '\"': buffer.append("&quot;"); break;
                case '\'': buffer.append("&apos;"); break;
                case '<': buffer.append("&lt;"); break;
                case '>': buffer.append("&gt;"); break;
                default: buffer.append(1, ch); break;
            }
        }
        return buffer;
    }

    // URL解码函数
    static std::string url_decode(const std::string& value) {
        std::string result;
        char ch;
        int i;
        for (i = 0; i < value.length(); i++) {
            if (value[i] == '%') {
                if (i + 2 < value.length()) {
                    std::istringstream hex(value.substr(i + 1, 2));
                    int hex_value;
                    if (hex >> std::hex >> hex_value) {
                        result += static_cast<char>(hex_value);
                        i += 2;
                    }
                }
            } else if (value[i] == '+') {
                result += ' '; // 将 '+' 替换为空格
            } else {
                result += value[i];
            }
        }
        return result;
    }

    //替换逻辑
    static void replace_placeholder(std::string& html, const std::string& placeholder, const std::string& value) {
        size_t pos = html.find(placeholder);
        while (pos != std::string::npos) {
            html.replace(pos, placeholder.length(), value);
            pos = html.find(placeholder, pos + value.length());
        }
    }
};


#endif//BLOG_HTMLHANDLE_HPP
