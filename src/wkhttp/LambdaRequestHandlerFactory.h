#pragma once

#include "HttpStatus.h"
#include "HttpServer.h"
#include "LambdaRequestHandler.h"
#include <proxygen/httpserver/RequestHandlerFactory.h>
#include <unordered_map>

namespace wk {
    class LambdaRequestHandlerFactory : public proxygen::RequestHandlerFactory {
    public:
        explicit LambdaRequestHandlerFactory(std::unordered_map<std::string, Handler> &handlers);

    public:
        void onServerStart(folly::EventBase *) noexcept override {};

        void onServerStop() noexcept override {};

        proxygen::RequestHandler *onRequest(proxygen::RequestHandler *next, proxygen::HTTPMessage *message) noexcept override;

    private:
        std::unordered_map<std::string, Handler> handlers;
    };
}




