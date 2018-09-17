cmake_minimum_required(VERSION 3.12)
project(marine_chart_tileserver)

set(CMAKE_CXX_STANDARD 17)

add_executable(marine_chart_tileserver
        src/http_server/EchoHandler.cpp
        src/http_server/EchoHandler.h
        src/http_server/EchoServer.cpp
        src/http_server/EchoStats.h
        src/Main.cpp src/Config.h)

target_link_libraries(marine_chart_tileserver -lfolly -lwangle -lproxygenhttpserver -lproxygenlib -lglog -lgflags -lboost_system)

if (APPLE)
    message("apple")
    set(OPENSSL_ROOT_DIR /usr/local/opt/openssl)
    target_link_libraries(marine_chart_tileserver -lboost_thread-mt)
    find_package(OpenSSL)
    if(OPENSSL_FOUND)
        target_link_libraries(marine_chart_tileserver ${OPENSSL_LIBRARIES})
        target_include_directories(marine_chart_tileserver PRIVATE ${OPENSSL_INCLUDE_DIR})
    else()
        message(FATAL_ERROR "openssl not found")
    endif()
endif()

if (LINUX)
    message("linux")
    target_link_libraries(marine_chart_tileserver -lboost_thread -lpthread)
endif()