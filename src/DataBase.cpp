//
// Created by CaoMengxuan on 2024/8/23.
//

#include "DataBase.hpp"

// 构造函数
DataBase::DataBase() : db(nullptr), rc(SQLITE_OK), lastError("") {}

// 析构函数
DataBase::~DataBase() {
    closeDB();  // 确保数据库关闭
}

// 打开数据库连接
int DataBase::connectDB(const std::string& DBname) const {
    return openDB(DBname);
}

int DataBase::openDB(const std::string& DBname) const {
    rc = sqlite3_open(DBname.c_str(), &db);
    if (rc) {
        lastError = sqlite3_errmsg(db);
        std::cerr << "无法打开数据库: " << lastError << std::endl;
        return rc;
    } else {
        std::cout << "数据库打开成功!" << std::endl;
    }
    return SQLITE_OK;
}

// 执行SQL命令
int DataBase::execCommand(const std::string& command) const {
    char* errmsg = nullptr;
    rc = sqlite3_exec(db, command.c_str(), nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK) {
        lastError = errmsg;
        std::cerr << "SQL错误: " << lastError << std::endl;
        sqlite3_free(errmsg);
        return rc;
    } else {
        std::cout << "SQL命令执行成功!" << std::endl;
    }
    return SQLITE_OK;
}

// 执行SQL查询并检索结果
std::vector<std::map<std::string, std::string>> DataBase::execQuery(const std::string& query) const {
    sqlite3_stmt* stmt;
    std::vector<std::map<std::string, std::string>> results;

    rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        lastError = sqlite3_errmsg(db);
        std::cerr << "准备语句失败: " << lastError << std::endl;
        return results;
    }

    int cols = sqlite3_column_count(stmt);
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        std::map<std::string, std::string> row;
        for (int col = 0; col < cols; col++) {
            std::string colName = sqlite3_column_name(stmt, col);
            std::string colValue = reinterpret_cast<const char*>(sqlite3_column_text(stmt, col));
            row[colName] = colValue;
        }
        results.push_back(row);
    }

    if (rc != SQLITE_DONE) {
        lastError = sqlite3_errmsg(db);
        std::cerr << "执行失败: " << lastError << std::endl;
    }

    sqlite3_finalize(stmt);
    return results;
}

// 关闭数据库连接
void DataBase::closeDB() const noexcept {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
        std::cout << "数据库关闭成功!" << std::endl;
    }
}

// 事务管理
int DataBase::beginTransaction() const {
    return execCommand("BEGIN TRANSACTION;");
}

int DataBase::commit() const {
    return execCommand("COMMIT;");
}

int DataBase::rollback() const {
    return execCommand("ROLLBACK;");
}

// 获取最后的错误信息
std::string DataBase::getLastError() const {
    return lastError;
}
void DataBase::operator->*(std::vector<std::map<std::string, std::string>>results) const noexcept {
    for (const auto& row : results) {
        std::cout << "ID: " << row.at("id") << ", Name: " << row.at("name") << std::endl;
    }
}
