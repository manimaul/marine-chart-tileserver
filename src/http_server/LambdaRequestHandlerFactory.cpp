#include "LambdaRequestHandlerFactory.h"

wk::LambdaRequestHandlerFactory::LambdaRequestHandlerFactory(std::unordered_map<std::string, wk::Handler> &handlers) : handlers(handlers) {}

proxygen::RequestHandler *
wk::LambdaRequestHandlerFactory::onRequest(proxygen::RequestHandler *, proxygen::HTTPMessage *message) noexcept {
    auto sig = proxygen::methodToString(message->getMethod().value()) + message->getPath();
    auto got = handlers.find(sig);
    if (got == handlers.end()) {
        Handler handler = [](HttpRequest &request) {
            return HttpResponse(HttpStatus::NotFound);
        };
        return new LambdaRequestHandler(handler);
    } else {
        return new LambdaRequestHandler(got->second);
    }
}
