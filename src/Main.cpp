
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

    wk::HttpServer(
            wk::Config()
                    .setIpAddress("0.0.0.0")
                    .setPort(8080)
                    .setIdleTimeoutSeconds(60))
            .addRoute("/", wk::HttpMethod::Get, [](wk::HttpRequest &request) {
                wk::HttpResponse response = wk::HttpResponse(wk::HttpStatus::Ok)
                        .addHeader("hi", "there")
                        .setBody("hello path slash");
                return response;
            })
            .addRoute("/foo", wk::HttpMethod::Get, [](wk::HttpRequest &request) {
                wk::HttpResponse response = wk::HttpResponse(wk::HttpStatus::Ok)
                        .addHeader("hi", "there")
                        .setBody("hello path foo");
                return response;
            })
            .listenAndServer();
}

