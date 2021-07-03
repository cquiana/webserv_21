cmake_minimum_required(VERSION 3.19)
project(webserv_21)

set(CMAKE_CXX_STANDARD 98)

add_executable(webserv
        main.cpp
        Webserv.cpp
        Client.cpp
        Request.cpp
        Responce.cpp
        utils.cpp
        Server.cpp)