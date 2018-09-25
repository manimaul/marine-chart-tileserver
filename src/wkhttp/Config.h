#pragma once

#include <string>

namespace wk {

    class Config {
    public:
        explicit Config() : ipAddress("0.0.0.0"), port(8080), idleTimeoutSeconds(60) {}

        std::string const &getIpAddress() const {
            return ipAddress;
        }

        Config& setIpAddress(std::string const &ipAddress) {
            Config::ipAddress = ipAddress;
            return *this;
        }

        uint16_t getPort() const {
            return port;
        }

        Config& setPort(uint16_t port) {
            Config::port = port;
            return *this;
        }

        uint8_t getIdleTimeoutSeconds() const {
            return idleTimeoutSeconds;
        }

        Config& setIdleTimeoutSeconds(uint8_t idleTimeoutSeconds) {
            Config::idleTimeoutSeconds = idleTimeoutSeconds;
            return *this;
        }

    private:
        std::string ipAddress;
        uint16_t port;
        uint8_t idleTimeoutSeconds;
    };
}
