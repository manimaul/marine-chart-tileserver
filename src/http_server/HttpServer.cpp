
#include "HttpServer.h"

#include <folly/Memory.h>
#include <folly/io/async/EventBaseManager.h>
#include <proxygen/httpserver/HTTPServer.h>
#include <proxygen/httpserver/RequestHandlerFactory.h>
#include <proxygen/httpserver/ResponseBuilder.h>

class RequestHandlerWrapper : public proxygen::RequestHandler {

public:

    RequestHandlerWrapper(wk::Handler &handler) : handler(handler), request() {}

    void onRequest(std::unique_ptr<proxygen::HTTPMessage> headers) noexcept override {
        headers->getHeaders().forEach([this] (auto &key, auto &value) {
            request.addHeader(key, value);
        });
    }

    void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override {
        std::string bodyStr = reinterpret_cast<const char*>(body->data());
        request.setBody(bodyStr);
    }

    void onEOM() noexcept override {
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

    void onUpgrade(proxygen::UpgradeProtocol proto) noexcept override {
        // handler doesn't support upgrades
    }

    void requestComplete() noexcept override {
        delete this;
    }

    void onError(proxygen::ProxygenError err) noexcept override {
        delete this;
    }

    wk::Handler handler;
    wk::HttpRequest request;
};

class RequestHandlerFactory : public proxygen::RequestHandlerFactory {
public:
    RequestHandlerFactory(std::unordered_map<std::string, wk::Handler> &handlers) : handlers(handlers) {}

public:
    void onServerStart(folly::EventBase * /*evb*/) noexcept override {
    }

    void onServerStop() noexcept override {
    }

    proxygen::RequestHandler *onRequest(proxygen::RequestHandler *, proxygen::HTTPMessage *message) noexcept override {
        std::string path = message->getPath();
        auto got = handlers.find(path);
        if (got == handlers.end()) {
            wk::Handler handler = [](wk::HttpRequest &request) {
                return wk::HttpResponse(wk::HttpStatus::NotFound);
            };
            return new RequestHandlerWrapper(handler);
        } else {
            return new RequestHandlerWrapper(got->second);
        }
    }

private:
    std::unordered_map<std::string, wk::Handler> handlers;
};

wk::HttpServer::HttpServer(wk::Config const &config) : config(config), handlers() {}

void wk::HttpServer::listenAndServer() {
    std::vector<proxygen::HTTPServer::IPConfig> IPs = {
            {folly::SocketAddress(config.getIpAddress(), config.getPort(), true), proxygen::HTTPServer::Protocol::HTTP},
            {folly::SocketAddress(config.getIpAddress(), 11001,
                                  true),                                          proxygen::HTTPServer::Protocol::HTTP2},
    };

    proxygen::HTTPServerOptions options;
    options.threads = static_cast<size_t>(sysconf(_SC_NPROCESSORS_ONLN));
    options.idleTimeout = std::chrono::seconds(config.getIdleTimeoutSeconds());
    options.shutdownOn = {SIGINT, SIGTERM};
    options.enableContentCompression = false;
    options.handlerFactories = proxygen::RequestHandlerChain()
            .addThen<RequestHandlerFactory>(handlers)
            .build();
    options.h2cEnabled = true;

    proxygen::HTTPServer server(std::move(options));
    server.bind(IPs);

    // Start HTTPServer mainloop in a separate thread
    std::thread t([&]() {
        server.start();
    });

    t.join();
}

wk::HttpServer &wk::HttpServer::addRoute(std::string const &routePattern,
                                         HttpMethod const &method,
                                         Handler handler) {
    handlers.emplace(routePattern, handler);
    return *this;
}
