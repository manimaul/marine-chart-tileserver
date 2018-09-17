#pragma once

#include <string>

namespace wk {

    class Config {
    public:
        explicit Config() : ipAddress("0.0.0.0"), port(8080) {}

        Config& setIpAddress(std::string const &ipAddress) {
            Config::ipAddress = ipAddress;
            return *this;
        }

        Config& setPort(uint16_t port) {
            Config::port = port;
            return *this;
        }

    private:
        std::string ipAddress;
        uint16_t port;

    };
}
