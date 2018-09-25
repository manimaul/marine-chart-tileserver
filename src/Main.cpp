
#include <iostream>
#include "wkhttp/Config.h"
#include "wkhttp/HttpRequest.h"

#include "wkhttp/HttpRequest.h"
#include "wkhttp/HttpResponse.h"
#include "wkhttp/HttpServer.h"
#include <glog/logging.h>

#define VERSION "0.0.1-SNAPSHOT"

int main(int argc, char *argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();

    wk::HttpServer(
            wk::Config()
                    .setIpAddress("0.0.0.0")
                    .setPort(8080)
                    .setIdleTimeoutSeconds(60))
            .addStaticContent("resources")
            .addRoute("/version", wk::HttpMethod::Post, [](wk::HttpRequest &request) {
                wk::HttpResponse response = wk::HttpResponse(wk::HttpStatus::Ok)
                        .setBody(VERSION);
                return response;
            })
            .listenAndServe();
}

