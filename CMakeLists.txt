cmake_minimum_required(VERSION 3.12)
project(vial)

set(CMAKE_CXX_STANDARD 17)

# Tests

add_subdirectory(tests)

# Vial
set(VIAL_SOURCES vial/HttpServer.cpp
        vial/HttpMessage.cpp
        vial/StaticHandler.cpp
        vial/Init.cpp
        vial/LambdaRequestHandler.cpp
        vial/LambdaRequestHandlerFactory.cpp
        vial/StaticContentHandlerFactory.cpp)
add_library(vial SHARED ${VIAL_SOURCES})
add_library(vial_static STATIC ${VIAL_SOURCES})

include_directories(vial/include)

set(VIAL_LINK_LIBS -lfolly
        -lwangle
        -lproxygenhttpserver
        -lproxygenlib
        -lglog
        -lgflags
        -lboost_system
        -lboost_filesystem)

find_package(OpenSSL)
if(OPENSSL_FOUND)
    set(VIAL_LINK_LIBS ${VIAL_LINK_LIBS} -lboost_thread -lpthread ${OPENSSL_LIBRARIES})
    target_link_libraries(vial ${VIAL_LINK_LIBS})
    target_link_libraries(vial_static ${VIAL_LINK_LIBS})
    target_include_directories(vial_static PRIVATE ${OPENSSL_INCLUDE_DIR})
else()
    message(FATAL_ERROR "openssl not found")
endif()

# Example

add_executable(vial_example example/Main.cpp)
target_link_libraries(vial_example vial)
target_include_directories(vial_example PRIVATE vial/include)
file(COPY example/resources DESTINATION "${CMAKE_BINARY_DIR}/vial-example")

# Installation

install(TARGETS vial
        LIBRARY DESTINATION lib)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/vial/include/
        DESTINATION include/vial
        FILES_MATCHING PATTERN "*.h*")