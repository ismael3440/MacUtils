#ifndef __SOCKETS_H__
#define __SOCKETS_H__

#include <array>
#include <cstdint>

std::array<uint8_t, 6> get_device_mac(std::string);
uint32_t get_device_ip(std::string);
bool set_device_mac(std::string, std::string);

#endif // __SOCKETS_H__
