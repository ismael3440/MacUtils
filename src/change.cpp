#include <print>
#include <vector>
#include <string>

#include <net/if.h>
#include <linux/if_arp.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

#include "utils.h"
#include "sockets.h"
#include "commands.h"
#include "errors.h"

int change(std::vector<std::string> args) {
    const auto parse = arg_parser(std::vector<std::string>(args.begin() + 2, args.end()));
    if (!parse.has_value()) {
        ErrorArgumentNotMatching(args[0], args[1], "<nueva_mac> <interfaz>");
        return -1;
    }
    const auto& arg_data = parse.value().first;
    const auto& arg_params = parse.value().second;

    std::string new_mac;
    std::string iface;

    for (const auto& param : arg_params) {
        if (param.first == "copy_of") {
            if (!iface_exists(param.second)) {
                ErrorIfaceNotExist(args[0], iface);
                return -1;
            }
            new_mac = mac_to_string(get_device_mac(param.second));
        }
    }

    for (const auto& arg : arg_data) {
        if (arg == "-random" && new_mac.empty()) {
            new_mac = get_random_mac();
        } else if (iface_exists(arg) && iface.empty()) {
            iface = arg;
        } else if (isMAC(arg) && new_mac.empty()) {
            new_mac = arg;
        }
    }

    if (iface.empty() || new_mac.empty()) {
        ErrorArgumentNotMatching(args[0], args[1], "<nueva_mac> <interfaz>");
        return -1;
    }

    std::array<uint8_t, 6> device_mac = get_device_mac(iface);
    if (device_mac.empty()) {
        ErrorGetDeviceMac(args[0], iface);
        return -1;
    }
    std::println("La dirección MAC {} está siendo cambiada por {} en {}...",
                 mac_to_string(device_mac), new_mac, iface);

    ifreq ifr;
    ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;
    strncpy(ifr.ifr_name, iface.c_str(), IFNAMSIZ);
    memcpy(ifr.ifr_hwaddr.sa_data, string_to_mac(new_mac).data(), 6);

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        ErrorRestoreSocket(args[0], iface);
        return -1;
    }

    if (ioctl(sock, SIOCSIFHWADDR, &ifr) < 0) {
        ErrorRestoreIoctl(args[0], iface);
        close(sock);
        return -1;
    }

    close(sock);
    std::println("Su dirección MAC fue cambiada exitosamente por {} en {}.",
                 new_mac, iface);
    return 0;
}
