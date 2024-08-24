//
// Created by CaoMengxuan on 2024/8/24.
//
#include "DataBase_storage.hpp"

// 在这里初始化单例对象
DataBase& dataBase = DataBase_storage::getInstance().getDB();