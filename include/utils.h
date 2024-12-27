#ifndef __UTILS_H__
#define __UTILS_H__

#include <map>
#include <string>
#include <array>
#include <utility>
#include <vector>

bool isMAC(std::string);
bool isIP(std::string);
bool iface_exists(std::string);

std::array<uint8_t, 6> string_to_mac(std::string);
std::string mac_to_string(std::array<uint8_t, 6>);

uint32_t string_to_ip(std::string);
std::string ip_to_string(uint32_t);

std::array<uint8_t, 6> get_device_mac(std::string);
uint32_t get_device_ip(std::string);

#endif // __UTILS_H__
