#pragma once


#include <memory>
#include <folly/io/IOBuf.h>
#include <proxygen/lib/http/HTTPMessage.h>

class HttpMessage {

public:
    explicit HttpMessage();

    std::string const &getBody() const;

    virtual HttpMessage  &setBody(std::string const &body);

    virtual std::unordered_map<std::string, std::string> const &getHeaders() const;

    virtual HttpMessage &addHeader(std::string const &key, std::string const &value);

protected:
    std::string body;
    std::unordered_map<std::string, std::string> headers;

};



