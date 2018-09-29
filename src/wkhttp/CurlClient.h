#pragma once

#include <folly/io/async/EventBase.h>
#include <folly/io/async/SSLContext.h>
#include <proxygen/lib/http/HTTPConnector.h>
#include <proxygen/lib/http/session/HTTPTransaction.h>
#include <proxygen/lib/utils/URL.h>

namespace wk {

    class CurlClient : public proxygen::HTTPConnector::Callback,
                       public proxygen::HTTPTransactionHandler {

    public:
        CurlClient(folly::EventBase* evb,
                   proxygen::HTTPMethod httpMethod,
                   proxygen::URL const &url,
                   proxygen::URL const *proxy,
                   proxygen::HTTPHeaders const &headers,
                   std::string const &inputFilename,
                   bool h2c = false,
                   unsigned short httpMajor = 1,
                   unsigned short httpMinor = 1);

        virtual ~CurlClient() = default;

        static proxygen::HTTPHeaders parseHeaderMap(std::unordered_map<std::string, std::string> const &headerMap);

        // initial SSL related structures
        void initializeSsl(std::string const &caPath,
                           std::string const &nextProtos,
                           std::string const &certPath = "",
                           std::string const &keyPath = "");
        void sslHandshakeFollowup(proxygen::HTTPUpstreamSession* session) noexcept;

        // HTTPConnector methods
        void connectSuccess(proxygen::HTTPUpstreamSession* session) override;
        void connectError(const folly::AsyncSocketException& ex) override;

        // HTTPTransactionHandler methods
        void setTransaction(proxygen::HTTPTransaction* txn) noexcept override;
        void detachTransaction() noexcept override;
        void onHeadersComplete(
                std::unique_ptr<proxygen::HTTPMessage> msg) noexcept override;
        void onBody(std::unique_ptr<folly::IOBuf> chain) noexcept override;
        void onTrailers(
                std::unique_ptr<proxygen::HTTPHeaders> trailers) noexcept override;
        void onEOM() noexcept override;
        void onUpgrade(proxygen::UpgradeProtocol protocol) noexcept override;
        void onError(proxygen::HTTPException const &error) noexcept override;
        void onEgressPaused() noexcept override;
        void onEgressResumed() noexcept override;

        void sendRequest(proxygen::HTTPTransaction* txn);

        // Getters
        folly::SSLContextPtr getSSLContext() { return sslContext_; }

        const std::string& getServerName() const;

        void setFlowControlSettings(size_t recvWindow);

        const proxygen::HTTPMessage* getResponse() const {
            return response_.get();
        }

        void setLogging(bool enabled) {
            loggingEnabled_ = enabled;
        }

    protected:
        proxygen::HTTPTransaction* txn_{nullptr};
        folly::EventBase* evb_{nullptr};
        proxygen::HTTPMethod httpMethod_;
        proxygen::URL url_;
        std::unique_ptr<proxygen::URL> proxy_;
        proxygen::HTTPMessage request_;
        const std::string inputFilename_;
        folly::SSLContextPtr sslContext_;
        size_t recvWindow_{0};
        bool loggingEnabled_{true};
        bool h2c_{false};
        uint8_t  httpMajor_;
        uint8_t  httpMinor_;

        std::unique_ptr<proxygen::HTTPMessage> response_;
    };

} // CurlService namespace