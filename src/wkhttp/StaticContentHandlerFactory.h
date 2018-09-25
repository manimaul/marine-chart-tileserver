#pragma once

#include "StaticHandler.h"

#include <proxygen/httpserver/RequestHandlerFactory.h>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>

namespace wk {
    class StaticContentHandlerFactory : public proxygen::RequestHandlerFactory {
    public:
        explicit StaticContentHandlerFactory(std::string const &dir);

        void onServerStart(folly::EventBase *) noexcept override {}

        void onServerStop() noexcept override {}

        proxygen::RequestHandler *onRequest(proxygen::RequestHandler *nextHandler, proxygen::HTTPMessage *message) noexcept override;

    private:
        boost::filesystem::path staticContentDir;
    };
}




