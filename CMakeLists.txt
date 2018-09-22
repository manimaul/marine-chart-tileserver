cmake_minimum_required(VERSION 3.12)
project(marine_chart_tileserver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INSTALL_PREFIX /opt/mct)

add_subdirectory(src/tests)

add_executable(marine_chart_tileserver
        src/Main.cpp
        src/http_server/Config.h
        src/http_server/HttpRequest.h
        src/http_server/HttpResponse.h
        src/http_server/HttpStatus.h
        src/http_server/HttpServer.cpp
        src/http_server/HttpServer.h
        src/http_server/HttpMethod.h
        src/http_server/HttpMessage.cpp
        src/http_server/HttpMessage.h
        src/http_server/StaticHandler.cpp
        src/http_server/StaticHandler.h
        src/http_server/LambdaRequestHandler.cpp
        src/http_server/LambdaRequestHandler.h
        src/http_server/LambdaRequestHandlerFactory.cpp
        src/http_server/LambdaRequestHandlerFactory.h
        src/http_server/StaticContentHandlerFactory.cpp
        src/http_server/StaticContentHandlerFactory.h)

target_link_libraries(marine_chart_tileserver
        -lfolly
        -lwangle
        -lproxygenhttpserver
        -lproxygenlib
        -lglog
        -lgflags
        -lboost_system
        -lboost_filesystem)

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

file(COPY resources DESTINATION "${CMAKE_BINARY_DIR}")

install(TARGETS marine_chart_tileserver DESTINATION bin)
