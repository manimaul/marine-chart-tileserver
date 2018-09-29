//#include <folly/io/async/EventBase.h>
#include "HttpClient.h"

wk::HttpResponse wk::HttpClient::call(wk::HttpCall &call) {

    return wk::HttpResponse(HttpStatus::Code::NotFound);
}

void wk::HttpClient::callAsync(wk::ClientHandler &handler) {
//    folly::EventBase eventBase;
}
