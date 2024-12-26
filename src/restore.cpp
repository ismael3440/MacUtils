#include <print>
#include <vector>
#include <string>

#include <net/if.h>
#include <linux/if_arp.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

#include "utils.h"
#include "commands.h"
#include "errors.h"

int restore(std::vector<std::string> args) {
    if (args.size() != 4) {
        ErrorArgumentNotMatching(args[0], args[1], "<nueva_mac> <interfaz>");
        return -1;
    }
    std::string new_mac = args[2];
    std::string iface = args[3];

    if (!iface_exists(iface)) {
        ErrorIfaceNotExist(args[0], iface);
        return -1;
    }

    if (!isMAC(new_mac)) {
        ErrorInvalidMacAddress(args[0], new_mac);
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
