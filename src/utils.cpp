#include <cstddef>
#include <optional>
#include <random>
#include <string>
#include <array>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <algorithm>

#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <utility>

#include "sockets.h"
#include "utils.h"

std::array<uint8_t, 6> string_to_mac(std::string mac_string) {
    std::array<uint8_t, 6> mac;
    std::stringstream stream_mac(mac_string);
    std::string mac_sec;

    int iterator = 0;
    while (std::getline(stream_mac, mac_sec, ':')) {
        uint8_t temp_byte = static_cast<uint8_t>(std::stoi(mac_sec, nullptr, 16));
        mac[iterator] = temp_byte;
        iterator = iterator < 5 ? iterator + 1 : 5;
    }
    return mac;
}

std::string mac_to_string(std::array<uint8_t, 6> mac) {
    std::ostringstream oss;
    for (size_t i = 0; i < mac.size(); i++) {
        if (i > 0)
            oss << ':';
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(mac[i]);
    }
    return oss.str();
}

uint32_t string_to_ip(std::string ip_string) {
    in_addr addr;
    if (inet_pton(AF_INET, ip_string.c_str(), &addr) != 1)
        return INADDR_NONE;
    return addr.s_addr;
}

std::string ip_to_string(uint32_t ip) {
    char ip_string[INET_ADDRSTRLEN];
    in_addr addr;
    addr.s_addr = ip;
    if (inet_ntop(AF_INET, &addr, ip_string, INET_ADDRSTRLEN) == nullptr)
        return std::string();
    return std::string(ip_string);
}

bool isMAC(std::string mac) {
    if (mac.length() != 17) return false;
    if (std::count(mac.begin(), mac.end(), ':') != 5) return false;

    std::stringstream ss(mac);
    std::string token;
    while (std::getline(ss, token, ':')) {
        if (token.length() != 2) return false;
        if (!std::all_of(token.begin(), token.end(), ::isxdigit)) return false;
    }
    return true;
}

bool isIP(std::string ip) {
    if (ip.length() > 15 || ip.length() < 7) return false;
    if (std::count(ip.begin(), ip.end(), '.') != 3) return false;

    std::stringstream ss(ip);
    std::string token;
    while (std::getline(ss, token, '.')) {
        if (token.empty()) return false;
        if (!std::all_of(token.begin(), token.end(), ::isdigit)) return false;
        if (token[0] == '0' && token.length() > 1) return false;
        try {
            int num = std::stoi(token);
            if (num < 0 || num > 255) return false;
        } catch (...) {
            return false;
        }
    }
    return true;
}

bool iface_exists(std::string iface) {
    if (iface.size() > IFNAMSIZ) return false;
    ifreq ifr;
    strncpy(ifr.ifr_name, iface.c_str(), IFNAMSIZ);

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
        return false;

    if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0) {
        close(sock);
        return false;
    }

    close(sock);
    return true;
}

std::optional<std::pair<std::vector<std::string>, std::map<std::string, std::string>>> arg_parser(std::vector<std::string> args) {
    if (args.empty())
        return std::nullopt;
    std::vector<std::string> data;
    std::map<std::string, std::string> params;

    for (const auto& arg : args) {
        if (arg.empty())
            return std::nullopt;
        if (arg[0] == '-') {
            auto deĺimiter_pos = arg.find('=');
            if (deĺimiter_pos == std::string::npos) {
                data.push_back(arg);
                continue;
            }
            auto key = arg.substr(1, deĺimiter_pos - 1);
            auto value = arg.substr(deĺimiter_pos + 1);
            if (key.empty() || value.empty()) {
                return std::nullopt;
            }
            params[key] = value;
        } else {
            data.push_back(arg);
        }
    }
    return std::make_pair(data, params);
}

std::string get_random_mac() {
    std::ostringstream oss;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 255);
    for (int i = 0; i < 6; i++) {
        if (i > 0)
            oss << ":";
        oss << std::hex << std::setw(2) << std::setfill('0') << distrib(gen);
    }
    return oss.str();
}
