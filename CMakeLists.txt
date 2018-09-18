cmake_minimum_required(VERSION 3.12)
project(marine_chart_tileserver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INSTALL_PREFIX /opt/mct)

add_executable(marine_chart_tileserver
        src/Main.cpp
        src/http_server/Config.h
        src/http_server/HttpRequest.h
        src/http_server/HttpResponse.h
        src/http_server/HttpStatus.h
        src/http_server/HttpServer.cpp
        src/http_server/HttpServer.h
        src/http_server/HttpMethod.h)

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

if (UNIX AND NOT APPLE)
    message("linux")
    target_link_libraries(marine_chart_tileserver -lboost_thread -lpthread)
endif()

install (TARGETS marine_chart_tileserver DESTINATION bin)
