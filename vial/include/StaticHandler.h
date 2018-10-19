#pragma once

#include <folly/Memory.h>
#include <folly/File.h>
#include <proxygen/httpserver/RequestHandler.h>
#include <boost/filesystem.hpp>

namespace vial {

    class StaticHandler : public proxygen::RequestHandler {
    public:
        explicit StaticHandler(boost::filesystem::path const &path);

        void onRequest(std::unique_ptr<proxygen::HTTPMessage> headers)
        noexcept override;

        void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override;

        void onEOM() noexcept override;

        void onUpgrade(proxygen::UpgradeProtocol proto) noexcept override;

        void requestComplete() noexcept override;

        void onError(proxygen::ProxygenError err) noexcept override;

        void onEgressPaused() noexcept override;

        void onEgressResumed() noexcept override;

    private:
        void readFile(folly::EventBase* evb);
        bool checkForCompletion();

        std::unique_ptr<folly::File> file_;
        bool readFileScheduled_{false};
        std::atomic<bool> paused_{false};
        bool finished_{false};
        boost::filesystem::path desiredPath;
    };

}