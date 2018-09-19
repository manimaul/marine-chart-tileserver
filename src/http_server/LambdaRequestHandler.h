#pragma once

#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>
#include "HttpServer.h"
#include "HttpRequest.h"

namespace wk {

    class LambdaRequestHandler : public proxygen::RequestHandler {

    public:

        explicit LambdaRequestHandler(wk::Handler &handler);

        void onRequest(std::unique_ptr<proxygen::HTTPMessage> headers) noexcept override;

        void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override;

        void onEOM() noexcept override;

        void onUpgrade(proxygen::UpgradeProtocol proto) noexcept override;

        void requestComplete() noexcept override;

        void onError(proxygen::ProxygenError err) noexcept override;

    private:
        wk::Handler handler;
        wk::HttpRequest request;
    };
}



