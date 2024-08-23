//
// Created by CaoMengxuan on 2024/8/23.
//

#ifndef BLOG_DATABASE_HPP
#define BLOG_DATABASE_HPP

#include <sqlite3.h>
#include <string>
#include <iostream>
#include <vector>
#include <map>

class DataBase {
public:
    DataBase();
    ~DataBase();

    int connectDB(const std::string& DBname) const;
    int openDB(const std::string& DBname) const;
    int execCommand(const std::string& command) const;
    std::vector<std::map<std::string, std::string>> execQuery(const std::string& query) const;
    void closeDB() const noexcept;

    int beginTransaction() const;
    int commit() const;
    int rollback() const;
    std::string getLastError() const;

    // 提供一个直接打印查询结果的接口
    void operator->*(std::vector<std::map<std::string, std::string>>results)const noexcept;

    void operator[](const std::string& command)const;

private:
    mutable sqlite3* db;
    mutable int rc;
    mutable std::string lastError;
};


#endif//BLOG_DATABASE_HPP
