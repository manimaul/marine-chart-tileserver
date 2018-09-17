/*
 *  Copyright (c) 2015-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
#include <folly/Memory.h>
#include <folly/io/async/EventBaseManager.h>
#include <folly/portability/GFlags.h>
#include <folly/portability/Unistd.h>
#include <proxygen/httpserver/HTTPServer.h>
#include <proxygen/httpserver/RequestHandlerFactory.h>

#include "EchoHandler.h"
#include "EchoStats.h"

using namespace EchoService;
using namespace proxygen;

using folly::EventBase;
using folly::EventBaseManager;
using folly::SocketAddress;

using Protocol = HTTPServer::Protocol;

class EchoHandlerFactory : public RequestHandlerFactory {
 public:
  void onServerStart(folly::EventBase* /*evb*/) noexcept override {
    stats_.reset(new EchoStats);
  }

  void onServerStop() noexcept override {
    stats_.reset();
  }

  RequestHandler* onRequest(RequestHandler*, HTTPMessage*) noexcept override {
    return new EchoHandler(stats_.get());
  }

 private:
  folly::ThreadLocalPtr<EchoStats> stats_;
};

int mainz(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();

  std::vector<HTTPServer::IPConfig> IPs = {
    {SocketAddress("0.0.0.0", 11000, true), Protocol::HTTP},
    {SocketAddress("0.0.0.0", 11001, true), Protocol::HTTP2},
  };

  HTTPServerOptions options;
  options.threads = static_cast<size_t>(sysconf(_SC_NPROCESSORS_ONLN));
  options.idleTimeout = std::chrono::milliseconds(60000);
  options.shutdownOn = {SIGINT, SIGTERM};
  options.enableContentCompression = false;
  options.handlerFactories = RequestHandlerChain()
      .addThen<EchoHandlerFactory>()
      .build();
  options.h2cEnabled = true;

  HTTPServer server(std::move(options));
  server.bind(IPs);

  // Start HTTPServer mainloop in a separate thread
  std::thread t([&] () {
    server.start();
  });

  t.join();
  return 0;
}
