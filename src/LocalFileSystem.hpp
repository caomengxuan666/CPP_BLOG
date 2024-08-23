//
// Created by CaoMengxuan on 2024/8/23.
//

#ifndef BLOG_LOCALFILESYSTEM_HPP
#define BLOG_LOCALFILESYSTEM_HPP

#include <sstream>
#include <fstream>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

struct Post {
    int id;
    std::string title;
    std::string content;
    std::string author;
};

class LocalFileSystem {
private:
    std::string file_path;
    std::vector<Post> posts;
    int next_id = 1;

public:
    // 构造函数，接受文件路径作为参数
    LocalFileSystem(const std::string& file_path) : file_path(file_path) {
        load_posts_from_file();
    }

    // 加载文件内容到内存
    void load_posts_from_file() {
        std::ifstream file(file_path);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << file_path << std::endl;
            return;
        }

        std::string line;
        Post post;
        while (std::getline(file, line)) {
            if (line.empty()) continue;

            if (line.find("ID:") == 0) {
                post.id = std::stoi(line.substr(3));
            } else if (line.find("Title:") == 0) {
                post.title = line.substr(6);
            } else if (line.find("Author:") == 0) {
                post.author = line.substr(7);
            } else if (line.find("Content:") == 0) {
                post.content = line.substr(8);
            } else if (line == "---") {
                posts.push_back(post);
                post = Post(); // 重置 post 对象
            }
        }

        if (!post.title.empty()) {
            posts.push_back(post); // 保存最后一个 post
        }

        next_id = posts.empty() ? 1 : posts.back().id + 1;

        file.close();
    }

    // 保存所有博客数据到文件
    void save_posts_to_file() {
        std::ofstream file(file_path);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << file_path << std::endl;
            return;
        }

        for (const auto& post : posts) {
            file << "ID:" << post.id << "\n";
            file << "Title:" << post.title << "\n";
            file << "Author:" << post.author << "\n";
            file << "Content:" << post.content << "\n";
            file << "---\n";
        }

        file.close();
    }

    // 添加新博客并保存到文件
    void add_post(const std::string& title, const std::string& content, const std::string& author) {
        Post new_post = { next_id++, title, content, author };
        posts.push_back(new_post);
        save_posts_to_file();
    }

    // 获取所有博客
    std::vector<Post> get_all_posts() const {
        return posts;
    }

    // 根据ID获取特定博客
    Post get_post_by_id(int id) const {
        for (const auto& post : posts) {
            if (post.id == id) return post;
        }
        throw std::runtime_error("Post not found");
    }
};



#endif//BLOG_LOCALFILESYSTEM_HPP
