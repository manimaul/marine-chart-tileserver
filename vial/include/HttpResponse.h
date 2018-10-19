#pragma once

#include <utility>
#include <vector>
#include <unordered_map>
#include "HttpStatus.h"
#include "HttpMessage.h"

namespace vial {
    class HttpResponse : public HttpMessage {
    public:
        explicit HttpResponse(HttpStatus::Code const status) : status(status), HttpMessage() {}

        HttpStatus::Code const status;

        HttpResponse &setBody(std::string const &body) override {
            HttpMessage::setBody(body);
            return *this;
        }

        HttpResponse &addHeader(std::string const &key, std::string const &value) override {
            HttpMessage::addHeader(key, value);
            return *this;
        }

    };
}
