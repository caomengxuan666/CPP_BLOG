cmake_minimum_required(VERSION 3.22.1)
project(blog)


set(CMAKE_CXX_STANDARD 17)
IF(WIN32)
    set(BOOST_ROOT "D:/MyLibs/boost_1_85_0/boost_lib/")
    # 设置sqllite3路径
    set(SQLITE3_ROOT "C:/sqlite")
    set(Boost_USE_STATIC_LIBS ON)    # 设置使用Boost静态库
    set(Boost_COMPILER "-mgw8")      #指定编译器版本
    set(Boost_ARCHITECTURE "-x64")   # 指定boost库架构为x64
    set(FMT_HEADER_ONLY ON)
ENDIF ()

set(PROJECT_FILES
    src/main.cpp
        src/server.hpp
        src/server.cpp
        src/session.cpp
        src/session.hpp
        src/router.cpp
        src/router.hpp
        src/post_storage.cpp
        src/post_storage.hpp
        src/HtmlHandle.hpp
        src/DataBase.hpp
        src/DataBase.cpp
)

#设置sqllite3的头文件和库文件搜索路径
include_directories(${SQLITE3_ROOT})
link_directories(${SQLITE3_ROOT})

# 查找Boost库
find_package(Boost REQUIRED COMPONENTS regex thread chrono)

link_libraries(libgdi32.a libwsock32.a libws2_32.a)

add_executable(blog ${PROJECT_FILES})

# 链接Boost库
target_link_libraries(blog PRIVATE ${Boost_LIBRARIES})

# 链接sqllite3库
target_link_libraries(blog PRIVATE sqlite3)