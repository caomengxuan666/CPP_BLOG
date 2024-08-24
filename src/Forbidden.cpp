#include "Forbidden.hpp"

// 定义静态成员变量
KeyWordList Forbidden::forbidden_keywords;
KeyWordMap Forbidden::WORD_MAP = {
        {"<script>", "&lt;script&gt;"},
        {"</script>", "&lt;/script&gt;"},
        {"<iframe>", "&lt;iframe&gt;"},
        {"</iframe>", "&lt;/iframe&gt;"},
        {"eval\\(", "disabled_eval("},  // 修正这里
        {"javascript:", "disabled_javascript:"},
        {"onerror=", "disabled_onerror="},
        {"onload=", "disabled_onload="},
        {"<img ", "&lt;img "},
        {R"(onerror\s*=)", "disabled_onerror="},
        {R"(onclick\s*=)", "disabled_onclick="},
        {"<a ", "&lt;a "},
        {"href=\"javascript:", "href=\"disabled_javascript:"},
};


void Forbidden::add_forbid_keyword(const std::string &keyword) noexcept {
    forbidden_keywords.emplace_back(keyword);
}

void Forbidden::init_forbidden_keywords(KeyWordMap wordMap) noexcept {
    std::for_each(wordMap.begin(), wordMap.end(), [](const std::pair<std::string, std::string> &pair) {
        Forbidden::add_forbid_keyword(pair.first);
    });
    std::cout << "初始化结束" << std::endl;
}

void Forbidden::init_forbidden_keywords() noexcept {
    std::for_each(WORD_MAP.begin(), WORD_MAP.end(), [](const std::pair<std::string, std::string> &pair) {
        Forbidden::add_forbid_keyword(pair.first);
    });
    std::cout << "初始化结束" << std::endl;
}

void Forbidden::reload_forbidden_keywords(KeyWordMap wordMap) {
    forbidden_keywords.clear();
    init_forbidden_keywords(std::move(wordMap));
}

std::string Forbidden::filter_content(const std::string &content) {
    std::string filtered_content = content;

    for (const auto &[key, value] : WORD_MAP) {
        try {
            std::regex pattern(key, std::regex_constants::icase);
            filtered_content = std::regex_replace(filtered_content, pattern, value);
        } catch (const std::regex_error &e) {
            std::cerr << "正则表达式错误: " << e.what() << " (" << key << ")" << std::endl;
        }
    }

    return filtered_content;
}
