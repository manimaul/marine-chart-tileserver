
#include "HttpServer.h"
#include "StaticHandler.h"
#include "StaticContentHandlerFactory.h"
#include "LambdaRequestHandler.h"
#include "LambdaRequestHandlerFactory.h"

#include <folly/Memory.h>
#include <folly/io/async/EventBaseManager.h>
#include <proxygen/httpserver/HTTPServer.h>
#include <proxygen/httpserver/RequestHandlerFactory.h>
#include <proxygen/httpserver/ResponseBuilder.h>

wk::HttpServer::HttpServer(wk::Config const &config) : config(config), handlers(), staticContentDir() {}

void wk::HttpServer::listenAndServe() {
    std::vector<proxygen::HTTPServer::IPConfig> IPs = {
            {folly::SocketAddress(config.getIpAddress(), config.getPort(), true), proxygen::HTTPServer::Protocol::HTTP},
            {folly::SocketAddress(config.getIpAddress(), 11001, true), proxygen::HTTPServer::Protocol::HTTP2},
    };

    proxygen::HTTPServerOptions options;
    options.threads = static_cast<size_t>(sysconf(_SC_NPROCESSORS_ONLN));
    options.idleTimeout = std::chrono::seconds(config.getIdleTimeoutSeconds());
    options.shutdownOn = {SIGINT, SIGTERM};
    options.enableContentCompression = false;
    auto builder = proxygen::RequestHandlerChain();
    if (staticContentDir.length() > 0) {
        builder.addThen<StaticContentHandlerFactory>(staticContentDir);
    }
    builder.addThen<LambdaRequestHandlerFactory>(handlers);

    options.handlerFactories = builder.build();

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

wk::HttpServer &wk::HttpServer::addStaticContent(std::string const &baseDirectory) {
    staticContentDir = std::string(baseDirectory);
    return *this;
}
