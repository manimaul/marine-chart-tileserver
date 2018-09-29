#include <utility>

#pragma once

#include "HttpMethod.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

namespace wk {

    class HttpCall {
    public:
        explicit HttpCall(HttpMethod method, HttpRequest request) : method(method), request(std::move(request)) {}

        HttpMethod getMethod() const {
            return method;
        }

        HttpRequest const &getRequest() const {
            return request;
        }

    private:
        HttpMethod method;
        HttpRequest request;
    };

    typedef std::function<HttpCall(HttpResponse const &response)> ClientHandler;

    class HttpClient {
    public:
        void callAsync(ClientHandler &handler);
        HttpResponse call(HttpCall &call);
    };
}



