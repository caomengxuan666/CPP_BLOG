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

    /**
     * @param DBname 数据库的名称，其实也就是路径，一个.db文件
     * @return 一个int 状态码rc
     */
    int connectDB(const std::string& DBname) const;

    /**
     * @param command SQL命令
     * @note 建议使用我重载中括号的版本来操作
     * @return 一个int状态码rc
     */
    int execCommand(const std::string& command) const;

    /**
     * @param command SQL命令
     * @note 重载一个中括号来执行SQL命令，因为这样比较好看
     * @return 一个int状态码rc
     */
    int operator[](const std::string& command)const;

    /**
     * @param query 用于SQL查询的语句，只有这个语句要返回字符串结果
     * @note 没有加限制，请不要使用除了查询语句外的其他语句
     * @return 一个vector<map<string, string>>的结果，可以用函数打印
     */
    std::vector<std::map<std::string, std::string>> execQuery(const std::string& query) const;

    /**
     * @brief 用来显示查询结果的遍历函数，可以打印结果
     * @note 调试的时候用
     */
    void operator->*(std::vector<std::map<std::string, std::string>>results)const noexcept;

    /**
     * @brief 关闭数据库
     * @note 可以不使用，析构函数会自动关闭，所有这个数据库最好设置为全局变量，这样不会出现内存泄漏
     */
    void closeDB() const noexcept;

    /**
     * @brief 开启事务
     * @return 一个int状态码rc
     */
    int beginTransaction() const;

    /**
     * @brief 提交事务
     * @return 一个int状态码rc
     */
    int commit() const;

    /**
     * @brief 回滚事务
     * @return 一个int状态码rc
     */
    int rollback() const;

    /**
     * @brief 获取最后一次错误信息
     * @return 一个string类型的错误信息
     */
    std::string getLastError() const;

private:
    // 打开数据库的底层接口函数
    int openDB(const std::string& DBname) const;
private:
    mutable sqlite3* db;
    mutable int rc;
    mutable std::string lastError;
};


#endif//BLOG_DATABASE_HPP
