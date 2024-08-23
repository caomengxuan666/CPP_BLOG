cmake_minimum_required(VERSION 3.22.1)

project(blog)

set(CMAKE_CXX_STANDARD 17)

if (WIN32)
    set(SQLite3_INCLUDE_DIR "C:/sqlite")
    set(SQLite3_LIBRARIES "C:/sqlite/libsqlite3.a")
    set(BOOST_ROOT "D:/MyLibs/boost_1_85_0/boost_lib/")
    set(Boost_USE_STATIC_LIBS ON)    # 设置使用Boost静态库
    set(Boost_COMPILER "-mgw8")      #指定编译器版本
    set(Boost_ARCHITECTURE "-x64")   # 指定boost库架构为x64
    set(FMT_HEADER_ONLY ON)
endif ()

# 设置项目文件
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
        src/DataBase.cpp
        src/DataBase.hpp
)

include_directories(${SQLite3_INCLUDE_DIR})
link_directories(${SQLite3_INCLUDE_DIR})

# 查找Boost库
find_package(Boost REQUIRED COMPONENTS regex thread chrono)

# 链接winsock库
if (WIN32)
    link_libraries(libgdi32.a libwsock32.a libws2_32.a)
endif ()


add_executable(blog ${PROJECT_FILES})

# 链接Boost库
target_link_libraries(blog PRIVATE ${Boost_LIBRARIES})

# 链接SQLLITE3
target_link_libraries(blog PRIVATE ${SQLite3_LIBRARIES})