#include <utility>

#include <utility>

#pragma once

#include "HttpMethod.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

namespace wk {

    class HttpCall {
    public:
        explicit HttpCall(std::string url, HttpMethod method, HttpRequest request) : url(std::move(url)), method(method), request(std::move(request)) {}

        HttpMethod getMethod() const {
            return method;
        }

        HttpRequest const &getRequest() const {
            return request;
        }

        const std::string &getUrl() const {
            return url;
        }

    private:
        HttpMethod method;
        HttpRequest request;
        std::string url;
    };

    typedef std::function<HttpCall(HttpResponse const &response)> ClientHandler;

    class HttpClient {
    public:
        void callAsync(HttpCall &call, ClientHandler &handler);
        HttpResponse call(HttpCall &call);
    };
}



