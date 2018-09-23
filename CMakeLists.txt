cmake_minimum_required(VERSION 3.12)
project(marine_chart_tileserver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INSTALL_PREFIX /opt/mct)

# http server
add_subdirectory(src/wkhttp)

# google tests
add_subdirectory(src/tests/googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
add_executable(runUnitTests src/tests/HttpServerTest.cpp)
target_link_libraries(runUnitTests gtest gtest_main wkhttp)

# example server
add_executable(exampleServer src/ExampleServer.cpp)
target_link_libraries(exampleServer wkhttp)

# application
add_executable(marine_chart_tileserver src/Main.cpp)
target_link_libraries(marine_chart_tileserver wkhttp)

# resources
file(COPY resources DESTINATION "${CMAKE_BINARY_DIR}")
install(TARGETS marine_chart_tileserver DESTINATION bin)
