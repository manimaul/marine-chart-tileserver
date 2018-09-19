#include "StaticHandler.h"
#include "HttpStatus.h"

#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>
#include <folly/io/async/EventBaseManager.h>
#include <folly/FileUtil.h>
#include <folly/executors/GlobalExecutor.h>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>


namespace wk {

    /**
     * Handles requests by serving the file named in path.  Only supports GET.
     * reads happen in a CPU thread pool since read(2) is blocking.
     * If egress pauses, file reading is also paused.
     */
    void StaticHandler::onRequest(std::unique_ptr<proxygen::HTTPMessage> headers) noexcept {
        if (headers->getMethod() != proxygen::HTTPMethod::GET) {
            proxygen::ResponseBuilder(downstream_)
                    .status(400, "Bad method")
                    .body("Only GET is supported")
                    .sendWithEOM();
            return;
        }

        try {
            // we do NOT read the path from the headers at this point as the path should been validated prior
            // https://www.owasp.org/index.php/Testing_Directory_traversal/file_include_(OTG-AUTHZ-001)
            file_ = std::make_unique<folly::File>(desiredPath.c_str());
        } catch (const std::system_error &ex) {
            proxygen::ResponseBuilder(downstream_)
                    .status(404, "Not Found")
                    .body(folly::to<std::string>("Could not find ", headers->getPath(),
                                                 " ex=", folly::exceptionStr(ex)))
                    .sendWithEOM();
        }
        proxygen::ResponseBuilder(downstream_)
                .status(200, "Ok")
                .send();
        readFileScheduled_ = true;
        folly::getCPUExecutor()->add(
                std::bind(&StaticHandler::readFile, this,
                          folly::EventBaseManager::get()->getEventBase()));
    }

    void StaticHandler::readFile(folly::EventBase *evb) {
        folly::IOBufQueue buf;
        while (file_ && !paused_) {
            // read 4k-ish chunks and foward each one to the client
            auto data = buf.preallocate(4000, 4000);
            auto rc = folly::readNoInt(file_->fd(), data.first, data.second);
            if (rc < 0) {
                // error
                VLOG(4) << "Read error=" << rc;
                file_.reset();
                evb->runInEventBaseThread([this] {
                    LOG(ERROR) << "Error reading file";
                    downstream_->sendAbort();
                });
                break;
            } else if (rc == 0) {
                // done
                file_.reset();
                VLOG(4) << "Read EOF";
                evb->runInEventBaseThread([this] {
                    proxygen::ResponseBuilder(downstream_)
                            .sendWithEOM();
                });
                break;
            } else {
                buf.postallocate(rc);
                evb->runInEventBaseThread([this, body = buf.move()]() mutable {
                    proxygen::ResponseBuilder(downstream_)
                            .body(std::move(body))
                            .send();
                });
            }
        }

        // Notify the request thread that we terminated the readFile loop
        evb->runInEventBaseThread([this] {
            readFileScheduled_ = false;
            if (!checkForCompletion() && !paused_) {
                VLOG(4) << "Resuming deferred readFile";
                onEgressResumed();
            }
        });
    }

    void StaticHandler::onEgressPaused() noexcept {
        // This will terminate readFile soon
        VLOG(4) << "StaticHandler paused";
        paused_ = true;
    }

    void StaticHandler::onEgressResumed() noexcept {
        VLOG(4) << "StaticHandler resumed";
        paused_ = false;
        // If readFileScheduled_, it will reschedule itself
        if (!readFileScheduled_ && file_) {
            readFileScheduled_ = true;
            folly::getCPUExecutor()->add(
                    std::bind(&StaticHandler::readFile, this,
                              folly::EventBaseManager::get()->getEventBase()));
        } else {
            VLOG(4) << "Deferred scheduling readFile";
        }
    }


    void StaticHandler::onBody(std::unique_ptr<folly::IOBuf> /*body*/) noexcept {
        // ignore, only support GET
    }

    void StaticHandler::onEOM() noexcept {
    }

    void StaticHandler::onUpgrade(proxygen::UpgradeProtocol /*protocol*/) noexcept {
        // handler doesn't support upgrades
    }

    void StaticHandler::requestComplete() noexcept {
        finished_ = true;
        paused_ = true;
        checkForCompletion();
    }

    void StaticHandler::onError(proxygen::ProxygenError /*err*/) noexcept {
        finished_ = true;
        paused_ = true;
        checkForCompletion();
    }

    bool StaticHandler::checkForCompletion() {
        if (finished_ && !readFileScheduled_) {
            VLOG(4) << "deleting StaticHandler";
            delete this;
            return true;
        }
        return false;
    }

    StaticHandler::StaticHandler(boost::filesystem::path const &path) : desiredPath(path) {}

}