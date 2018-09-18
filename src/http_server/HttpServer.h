#pragma once

#include <functional>
#include "Config.h"
#include "HttpMethod.h"
#include "HttpRequest.h"
#include "HttpResponse.h"


namespace wk {

    typedef std::function<wk::HttpResponse(wk::HttpRequest &request)> Handler;

    class HttpServer {
    public:
        HttpServer(Config const &config);

        HttpServer &addRoute(std::string const &routePattern,
                             HttpMethod const &method,
                             Handler handler);

        void listenAndServer();

    private:
        Config const config;
        std::unordered_map<std::string, wk::Handler> handlers;
    };
}