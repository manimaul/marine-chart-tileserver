#include "LambdaRequestHandler.h"


vial::LambdaRequestHandler::LambdaRequestHandler(vial::Handler &handler) : handler(handler), request() {}

void vial::LambdaRequestHandler::onRequest(std::unique_ptr<proxygen::HTTPMessage> headers) noexcept {
    headers->getHeaders().forEach([this](auto &key, auto &value) {
        request.addHeader(key, value);
    });
}

void vial::LambdaRequestHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {
    std::string bodyStr = reinterpret_cast<const char *>(body->data());
    request.setBody(bodyStr);
}

void vial::LambdaRequestHandler::onEOM() noexcept {
    auto request = vial::HttpRequest();
    vial::HttpResponse response = handler(request);
    auto builder = proxygen::ResponseBuilder(downstream_);
    builder.status(response.status, vial::HttpStatus::reasonPhrase(response.status))
            .body(folly::IOBuf::copyBuffer(response.getBody()));
    for (auto const &pair : response.getHeaders()) {
        builder.header(pair.first, pair.second);
    }
    builder.sendWithEOM();
}

void vial::LambdaRequestHandler::onUpgrade(proxygen::UpgradeProtocol proto) noexcept {
    // handler doesn't support upgrades
}

void vial::LambdaRequestHandler::requestComplete() noexcept {
    delete this;
}

void vial::LambdaRequestHandler::onError(proxygen::ProxygenError err) noexcept {
    delete this;
}