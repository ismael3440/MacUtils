#include <array>
#include <string>

#include <unistd.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <linux/if_arp.h>

#include "sockets.h"
#include "utils.h"

std::array<uint8_t, 6> get_device_mac(std::string device) {
    ifreq ifr;
    strncpy(ifr.ifr_name, device.c_str(), IFNAMSIZ);

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
        return std::array<uint8_t, 6>();

    if (ioctl(sock, SIOCGIFHWADDR, &ifr) < 0) {
        close(sock);
        return std::array<uint8_t, 6>();
    }

    close(sock);
    std::array<uint8_t, 6> mac;
    std::memcpy(mac.data(), ifr.ifr_hwaddr.sa_data, mac.size());
    return mac;
}

uint32_t get_device_ip(std::string device) {
    ifreq ifr;
    strncpy(ifr.ifr_name, device.c_str(), IFNAMSIZ);
    auto& addr = *reinterpret_cast<sockaddr_in*>(&ifr.ifr_addr);

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
        return INADDR_NONE;

    if (ioctl(sock, SIOCGIFADDR, &ifr) < 0) {
        close(sock);
        return INADDR_NONE;
    }

    close(sock);
    auto ip = addr.sin_addr;
    return ip.s_addr;
}

bool set_device_mac(std::string mac, std::string iface) {
    ifreq ifr;
    ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;
    strncpy(ifr.ifr_name, iface.c_str(), IFNAMSIZ);
    std::memcpy(ifr.ifr_hwaddr.sa_data, string_to_mac(mac).data(), 6);

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
        return false;

    if(ioctl(sock, SIOCSIFHWADDR, &ifr) < 0) {
        close(sock);
        return false;
    }

    close(sock);
    return true;
}
