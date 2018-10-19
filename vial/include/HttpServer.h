#pragma once

#include <functional>
#include "Config.h"
#include "HttpMethod.h"
#include "HttpRequest.h"
#include "HttpResponse.h"


namespace vial {

    typedef std::function<vial::HttpResponse(vial::HttpRequest &request)> Handler;

    class HttpServer {
    public:
        explicit HttpServer(Config const &config);

        HttpServer &addRoute(std::string const &routePattern,
                             HttpMethod const &method,
                             Handler handler);

        HttpServer &addStaticContent(std::string const &baseDirectory);

        void listenAndServe();

    private:
        Config const config;
        std::unordered_map<std::string, vial::Handler> handlers;
        std::string staticContentDir; //todo: (WK) turn into a set
    };
}