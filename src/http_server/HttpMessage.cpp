#include "HttpMessage.h"


HttpMessage::HttpMessage() : headers(), body() {
}

std::string const &HttpMessage::getBody() const {
    return body;
}

HttpMessage &HttpMessage::setBody(std::string const &body) {
    HttpMessage::body = body;
    return *this;
}

std::unordered_map<std::string, std::string> const &HttpMessage::getHeaders() const {
    return headers;
}

HttpMessage &HttpMessage::addHeader(std::string const &key, std::string const &value) {
    return *this;
}
