#include "LambdaRequestHandler.h"


wk::LambdaRequestHandler::LambdaRequestHandler(wk::Handler &handler) : handler(handler), request() {}

void wk::LambdaRequestHandler::onRequest(std::unique_ptr<proxygen::HTTPMessage> headers) noexcept {
    headers->getHeaders().forEach([this](auto &key, auto &value) {
        request.addHeader(key, value);
    });
}

void wk::LambdaRequestHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {
    std::string bodyStr = reinterpret_cast<const char *>(body->data());
    request.setBody(bodyStr);
}

void wk::LambdaRequestHandler::onEOM() noexcept {
    auto request = wk::HttpRequest();
    wk::HttpResponse response = handler(request);
    auto builder = proxygen::ResponseBuilder(downstream_);
    builder.status(response.status, wk::HttpStatus::reasonPhrase(response.status))
            .body(folly::IOBuf::copyBuffer(response.getBody()));
    for (auto const &pair : response.getHeaders()) {
        builder.header(pair.first, pair.second);
    }
    builder.sendWithEOM();
}

void wk::LambdaRequestHandler::onUpgrade(proxygen::UpgradeProtocol proto) noexcept {
    // handler doesn't support upgrades
}

void wk::LambdaRequestHandler::requestComplete() noexcept {
    delete this;
}

void wk::LambdaRequestHandler::onError(proxygen::ProxygenError err) noexcept {
    delete this;
}