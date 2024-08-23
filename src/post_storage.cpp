#include "post_storage.hpp"

PostStorage& PostStorage::get_instance() {
    static PostStorage instance;
    return instance;
}

std::vector<BlogPost> PostStorage::get_all_posts() {
    std::lock_guard<std::mutex> lock(mutex_);
    return posts_;
}

BlogPost PostStorage::get_post_by_id(int id) {
    std::lock_guard<std::mutex> lock(mutex_);
    for (const auto& post : posts_) {
        if (post.id == id) {
            return post;
        }
    }
    return {0, "Not Found", "The requested post does not exist."};
}

void PostStorage::add_post(const std::string& title, const std::string& content, const std::string& author,const std::string &time) {
    std::lock_guard<std::mutex> lock(mutex_);
    posts_.push_back({next_id_++, title, content,author,time});
}
