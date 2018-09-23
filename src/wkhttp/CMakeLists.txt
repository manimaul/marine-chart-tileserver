cmake_minimum_required(VERSION 3.12)
project(wkhttp)

add_library(wkhttp STATIC
        Config.h
        HttpRequest.h
        HttpResponse.h
        HttpStatus.h
        HttpServer.cpp
        HttpServer.h
        HttpMethod.h
        HttpMessage.cpp
        HttpMessage.h
        StaticHandler.cpp
        StaticHandler.h
        LambdaRequestHandler.cpp
        LambdaRequestHandler.h
        LambdaRequestHandlerFactory.cpp
        LambdaRequestHandlerFactory.h
        StaticContentHandlerFactory.cpp
        StaticContentHandlerFactory.h)

set(MCT_LIBS
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
    find_package(OpenSSL)
    if(OPENSSL_FOUND)
        set(MCT_LIBS ${MCT_LIBS} ${OPENSSL_LIBRARIES} -lboost_thread-mt)
        target_include_directories(wkhttp PUBLIC ${OPENSSL_INCLUDE_DIR})
    else()
        message(FATAL_ERROR "openssl not found")
    endif()
endif()

if (UNIX AND NOT APPLE)
    message("linux")
    set(MCT_LIBS ${MCT_LIBS} -lboost_thread -lpthread)
endif()

target_link_libraries(wkhttp ${MCT_LIBS})
