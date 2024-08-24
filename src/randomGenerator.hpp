//
// Created by CaoMengxuan on 2024/8/24.
//

#ifndef BLOG_RANDOMGENERATOR_HPP
#define BLOG_RANDOMGENERATOR_HPP

#include <random>

static int generate_random_id()
{
    // 创建随机数引擎
    std::random_device rd; // 获取一个随机种子
    std::mt19937 gen(rd()); // 使用 Mersenne Twister 引擎

    // 定义分布范围
    std::uniform_int_distribution<> dist(0, INT32_MAX);

    // 生成随机数
    return dist(gen);
}

#endif//BLOG_RANDOMGENERATOR_HPP
