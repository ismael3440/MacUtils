#include <iostream>
#include <print>
#include <vector>
#include <string>

#include <linux/if_arp.h>
#include <arpa/inet.h>
#include <net/if.h>

#include "utils.h"
#include "commands.h"
#include "errors.h"

int search(std::vector<std::string> args) {
    if (args.size() != 4) {
        ErrorArgumentNotMatching(args[0], args[1], "<ip_objetivo> <interfaz>");
        return -1;
    }
    auto dest = std::string(args[2]);
    auto iface = std::string(args[3]);

    if (!iface_exists(iface)) {
        ErrorIfaceNotExist(args[0], iface);
        return -1;
    }

    if(!isIP(dest)) {
        ErrorInvalidIpAddress(args[0], dest);
        return -1;
    }

    struct [[gnu::packed]] {
        std::array<uint8_t, 6> hsrc;
        std::array<uint8_t, 6> hdst;
        uint16_t proto;
    } ethhdr;

    struct [[gnu::packed]] {
        uint16_t hrd_type;
        uint16_t pro_type;
        uint8_t hrd_len;
        uint8_t pro_len;
        uint16_t op;
        std::array<uint8_t, 6> hsrc;
        uint32_t psrc;
        std::array<uint8_t, 6> hdst;
        uint32_t pdst;
    } arphdr;

    std::array<uint8_t, sizeof(ethhdr) + sizeof(arphdr)> buffer;
    auto& eth = *reinterpret_cast<decltype(&ethhdr)>(buffer.data());
    auto& arp = *reinterpret_cast<decltype(&arphdr)>(buffer.data() + sizeof(ethhdr));

    eth.hsrc = get_device_mac(iface);
    if (eth.hsrc.empty()) {
        ErrorGetDeviceMac(args[0], iface);
        return -1;
    }

    std::fill(eth.hdst.begin(), eth.hdst.end(), 0xff);
    eth.proto = htons(ETH_P_ARP);

    arp.hrd_type = htons(ARPHRD_ETHER);
    arp.pro_type = htons(ETH_P_IP);

    arp.hrd_len = 6;
    arp.pro_len = 4;
    arp.op = htons(1);

    arp.hsrc = get_device_mac(iface);
    if (arp.hsrc.empty()) {
        ErrorGetDeviceMac(args[0], iface);
        return -1;
    }

    arp.psrc = get_device_ip(iface);
    if (arp.psrc == INADDR_NONE) {
        ErrorGetDeviceIp(args[0], iface);
        return -1;
    }

    std::fill(arp.hdst.begin(), arp.hdst.end(), 0x00);

    arp.pdst = string_to_ip(dest);
    if (arp.pdst == INADDR_NONE) {
        ErrorStringToIp(args[0], dest);
        return -1;
    }

    sockaddr_ll sll;
    sll.sll_family = AF_PACKET;
    sll.sll_protocol = htons(ETH_P_ARP);
    sll.sll_ifindex = if_nametoindex(iface.c_str());

    auto sll_len = static_cast<socklen_t>(sizeof(sll));

    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if (sock < 0) {
        ErrorSocketRaw(args[0], iface);
        return -1;
    }

    if (sendto(sock, buffer.data(), BUFSIZ, 0, reinterpret_cast<sockaddr*>(&sll), sll_len) < 0) {
        ErrorSendTo(args[0], iface);
        close(sock);
        return -1;
    }

    std::fill(buffer.begin(), buffer.end(), 0);

    timeval timeout {10, 0};
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        ErrorSockConfigTimeOut(args[0], iface);
        close(sock);
        return -1;
    }

    if (recvfrom(sock, buffer.data(), BUFSIZ, 0, reinterpret_cast<sockaddr*>(&sll), &sll_len) < 0) {
        ErrorRecvFrom(args[0], iface);
        close(sock);
        return -1;
    }

    std::print("Proceso terminado exitosamente.\n"
               "Dirección IP del objetivo: {}\n"
               "Dirección MAC del objetivo: {}\n",
               ip_to_string(arp.psrc),
               mac_to_string(arp.hsrc));

    close(sock);
    return 0;
}
