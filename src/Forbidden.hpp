// Created by CaoMengxuan on 2024/8/24.

#ifndef BLOG_FORBIDDEN_HPP
#define BLOG_FORBIDDEN_HPP

#include <algorithm>
#include <exception>
#include <regex>
#include <string>
#include <vector>
#include <map>
#include <iostream>

using KeyWordList = std::vector<std::string>;
using KeyWordMap = std::map<std::string, std::string>;

class Forbidden {
public:
    // 动态增加禁止关键词
    static void add_forbid_keyword(const std::string &keyword) noexcept;

    // 过滤输入内容
    static std::string filter_content(const std::string &content);

    // 初始化禁止关键词
    static void init_forbidden_keywords(KeyWordMap wordMap) noexcept;
    static void init_forbidden_keywords() noexcept;

    // 重新加载禁止关键词
    static void reload_forbidden_keywords(KeyWordMap wordMap);

private:
    // 静态成员变量
    static KeyWordList forbidden_keywords;
    static KeyWordMap WORD_MAP;  // 声明静态成员变量
};

#endif // BLOG_FORBIDDEN_HPP
