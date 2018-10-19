
#include <iostream>
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

