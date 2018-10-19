# Vial Native

Nothing to see here - this is just my sandbox for messing about with proxygen

[![CircleCI](https://circleci.com/gh/manimaul/vial-native.svg?style=svg)](https://circleci.com/gh/manimaul/vial-native)

# Getting Started
```
git submodule update --init

# build
mkdir build && \
cd build && \
cmake .. \
&& make

# run unit tests
./tests/runUnitTests

# run example
./vial_example

# install
make install
```

# Example

```cpp
#include "vial.h"

int main(int argc, char *argv[]) {
    vial::Init::init(&argc, &argv);

    vial::HttpServer(
            vial::Config()
                    .setIpAddress("0.0.0.0")
                    .setPort(8080)
                    .setIdleTimeoutSeconds(60))
            .addStaticContent("vial-example/resources")
            .addRoute("/foo", vial::HttpMethod::Post, [](vial::HttpRequest &request) {
                vial::HttpResponse response = vial::HttpResponse(vial::HttpStatus::Ok)
                        .addHeader("hi", "there")
                        .setBody("hello path POST foo");
                return response;
            })
            .addRoute("/foo", vial::HttpMethod::Get, [](vial::HttpRequest &request) {
                vial::HttpResponse response = vial::HttpResponse(vial::HttpStatus::Ok)
                        .addHeader("hi", "there")
                        .setBody("hello path GET foo");
                return response;
            })
            .addRoute("/baz", vial::HttpMethod::Get, [](vial::HttpRequest &request) {
                vial::HttpResponse response = vial::HttpResponse(vial::HttpStatus::Ok)
                        .addHeader("hi", "there")
                        .setBody("hello path GET baz");
                return response;
            })
            .listenAndServe();
} 
```
