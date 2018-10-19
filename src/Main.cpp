
#include <iostream>
#include "http_server/Config.h"
#include "http_server/HttpRequest.h"

#include "http_server/HttpRequest.h"
#include "http_server/HttpResponse.h"
#include "http_server/HttpServer.h"
#include <glog/logging.h>

int main(int argc, char *argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();

    vial::HttpServer(
            vial::Config()
                    .setIpAddress("0.0.0.0")
                    .setPort(8080)
                    .setIdleTimeoutSeconds(60))
            .addStaticContent("resources")
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

