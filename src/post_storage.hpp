#ifndef POST_STORAGE_HPP
#define POST_STORAGE_HPP

#include <string>
#include <vector>
#include <mutex>

struct BlogPost {
    int id;
    std::string title;
    std::string content;
    std::string author;
    std::string created_at;
};

class PostStorage {
public:
    static PostStorage& get_instance();

    std::vector<BlogPost> get_all_posts();
    BlogPost get_post_by_id(int id);
    void add_post(const std::string& title, const std::string& content,const std::string& author, const std::string& created_at);

private:
    PostStorage() = default;
    std::vector<BlogPost> posts_;
    int next_id_ = 1;
    std::mutex mutex_;
};

#endif
