#include "StaticContentHandlerFactory.h"

wk::StaticContentHandlerFactory::StaticContentHandlerFactory(std::string const &dir)  {
    auto path = boost::filesystem::path(dir);
    if (boost::filesystem::exists(path)) {
        staticContentDir = boost::filesystem::canonical(path);
    } else {
        staticContentDir = boost::filesystem::path();
    }
}

proxygen::RequestHandler *
wk::StaticContentHandlerFactory::onRequest(proxygen::RequestHandler *nextHandler, proxygen::HTTPMessage *message) noexcept  {
    if (!staticContentDir.empty() && message->getMethod() == proxygen::HTTPMethod::GET) {
        auto desiredPath = boost::filesystem::unique_path(staticContentDir).append(message->getPath());
        if (boost::filesystem::is_directory(desiredPath)) {
            desiredPath.append("/index.html");
        }
        if (boost::filesystem::exists(desiredPath)) {
            auto absPath = boost::filesystem::canonical(desiredPath);
            // validate that this path does NOT traverse out of our intended content directory
            // https://www.owasp.org/index.php/Testing_Directory_traversal/file_include_(OTG-AUTHZ-001)
            if (boost::starts_with(absPath.string(), staticContentDir.string())) {
                return new wk::StaticHandler(desiredPath);
            }
        }
    }
    return nextHandler;
}
