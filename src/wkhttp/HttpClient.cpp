#include <folly/io/async/EventBase.h>
#include "HttpClient.h"
#include "CurlClient.h"

wk::HttpResponse wk::HttpClient::call(wk::HttpCall &call) {

    return wk::HttpResponse(HttpStatus::Code::NotFound);
}

void wk::HttpClient::callAsync(HttpCall &call, wk::ClientHandler &handler) {
    folly::EventBase evb;
    proxygen::URL url(call.getUrl());
//    proxygen::URL proxy("");

//    if (FLAGS_http_method != "GET" && FLAGS_http_method != "POST") {
//        LOG(ERROR) << "http_method must be either GET or POST";
//        return EXIT_FAILURE;
//    }

    proxygen::HTTPMethod httpMethod = *proxygen::stringToMethod(httpMethodString(call.getMethod()));
//    if (httpMethod == proxygen::HTTPMethod::POST) {
//        try {
//            folly::File f(FLAGS_input_filename);
//            (void)f;
//        } catch (const std::system_error& se) {
//            LOG(ERROR) << "Couldn't open file for POST method";
//            LOG(ERROR) << se.what();
//            return EXIT_FAILURE;
//            return;
//        }
//    }

    proxygen::HTTPHeaders headers = CurlClient::parseHeaderMap(call.getRequest().getHeaders());

    CurlClient curlClient(&evb,
                          httpMethod,
                          url,
                          nullptr, //HTTP proxy url
                          headers,
                          "", //Filename to read from for POST requests
                          true); //Attempt HTTP/1.1 -> HTTP/2 upgrade
    // for spdy curlClient.setFlowControlSettings(FLAGS_recv_window);

    folly::SocketAddress addr = folly::SocketAddress(url.getHost(), url.getPort(), true);
    LOG(INFO) << "Trying to connect to " << addr;

    // Note: HHWheelTimer is a large object and should be created at most
    // once per thread
    folly::HHWheelTimer::UniquePtr timer{folly::HHWheelTimer::newTimer(
            &evb,
            std::chrono::milliseconds(folly::HHWheelTimer::DEFAULT_TICK_INTERVAL),
            folly::AsyncTimeout::InternalEnum::NORMAL,
            std::chrono::milliseconds(5000))};
    proxygen::HTTPConnector connector(&curlClient, timer.get());
//    if (!FLAGS_plaintext_proto.empty()) {
//        connector.setPlaintextProtocol(FLAGS_plaintext_proto);
//    }
    static const folly::AsyncSocket::OptionMap opts{{{SOL_SOCKET, SO_REUSEADDR}, 1}};

    if (url.isSecure()) {
//        curlClient.initializeSsl(
//                FLAGS_ca_path, FLAGS_next_protos, FLAGS_cert_path, FLAGS_key_path);
//        connector.connectSSL(
//                &evb,
//                addr,
//                curlClient.getSSLContext(),
//                nullptr,
//                std::chrono::milliseconds(15000),
//                opts,
//                folly::AsyncSocket::anyAddress(),
//                curlClient.getServerName());
    } else {
        connector.connect(&evb, addr,
                          std::chrono::milliseconds(15000), opts);
    }

    evb.loop();

}
