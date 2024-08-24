//
// Created by CaoMengxuan on 2024/8/24.
//

#ifndef BLOG_DATABASE_STORAGE_HPP
#define BLOG_DATABASE_STORAGE_HPP

#include "DataBase.hpp"
#include "post_storage.hpp"

//单例模式，全局只有一个数据库连接
class DataBase_storage {
public:
    DataBase_storage(const DataBase_storage &) = delete;
    DataBase_storage &operator=(const DataBase_storage &) = delete;
    DataBase_storage(DataBase_storage &&) = delete;
    static DataBase_storage &getInstance() {
        static DataBase_storage instance;
        return instance;
    }

    //增加博客文章
    void add_post(const std::string &title, const std::string &content, const std::string &author_name, const std::string &create_time) {
        //SQLITE
        db.beginTransaction();
        auto rc=db["INSERT INTO passage (title, content, author_name, create_time) VALUES ('" + title + "', '" + content + "', '" + author_name + "', '" + create_time + "')"];
        //检测是否成功
        if (rc != SQLITE_OK) {
            std::cout << "添加博客文章失败！: " << db.getLastError() << std::endl;
            db.rollback();
        } else {
            std::cout<<"成功添加博客文章！"<<std::endl;
            db.commit();
        }
    }

    void delete_post(int id) {
        db.beginTransaction();
        std::string command = "DELETE FROM passage WHERE passage_index = " + std::to_string(id);
        db.execCommand(command);
    }

    void load_posts_from_dataBase() {
        //后期优化，需要添加除了content以外的所有的信息
        auto result = db.execQuery("SELECT * FROM passage");
        for (auto &row: result) {
            //将数据存储到PostStorage中
            // 将新文章添加到存储中
            PostStorage::get_instance().add_post(row["title"], row["content"], row["author_name"], row["create_time"]);
        }
    }

    //为了后期优化，专门在用户点击的时候再去加载content信息
    void read_post(int id) {
        auto result = db.execQuery("SELECT * FROM passage WHERE passage_index=" + std::to_string(id));
        PostStorage::get_instance().add_post(result[0]["title"], result[0]["content"], result[0]["author_name"], result[0]["create_time"]);
    }

    DataBase &getDB() {
        return db;
    }


private:
    DataBase db;

    DataBase_storage() {
        db.connectDB("../blog_files/blog.db");
    }

    ~DataBase_storage() {
        db.closeDB();
    }
};


#endif//BLOG_DATABASE_STORAGE_HPP
