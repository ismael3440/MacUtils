#include <string>
#include <print>

#include <cerrno>
#include <cstring>

#include "errors.h"

void ErrorArgumentNotMatching(std::string filePath, std::string arg, std::string params) {
    std::print("{}: Los argumentos pasados son incorrectos.\n"
               "{} {}\n"
               "Use /help para más información.\n",
               filePath, arg, params);
}

void ErrorIfaceNotExist(std::string filePath, std::string iface) {
    std::println("{}: La interfaz {} no está disponible.",
                 filePath, iface);
}

void ErrorInvalidMacAddress(std::string filePath, std::string mac) {
    std::println("{}: La dirección MAC {} no tiene un formato adecuado.",
                 filePath, mac);
}

void ErrorInvalidIpAddress(std::string filePath, std::string ip) {
    std::println("{}: La dirección IP {} no tiene un formato adecuado.",
                 filePath, ip);
}

void ErrorSocketRaw(std::string filePath, std::string iface) {
    std::print("{}: Ha ocurrido un error al crear el socket crudo en la interfaz {}\n"
               "SocketRaw: {}\n",
               filePath, iface, std::strerror(errno));
}

void ErrorSendTo(std::string filePath, std::string iface) {
    std::print("{}: Ha ocurrido un error al enviar los datos a través de la interfaz {}\n"
               "SendTo: {}",
               filePath, iface, std::strerror(errno));
}

void ErrorRecvFrom(std::string filePath, std::string iface) {
    std::print("{}: Ha ocurrido un error al recibir el paquete a través de la interfaz de red {}\n"
               "RecvFrom: {}\n",
               filePath, iface, std::strerror(errno));
}

void ErrorSockConfigTimeOut(std::string filePath, std::string iface) {
    std::print("{}: A ocurrido un error al configurar un tiempo de espera en la interfaz {}\n"
               "TimeOut: {}\n",
               filePath, iface, std::strerror(errno));
}

void ErrorGetDeviceMac(std::string filePath, std::string iface) {
    std::print("{}: No se pudo obtener la dirección MAC de este dispositivo en la interfaz {}\n"
               "GetDeviceMAC: {}\n",
               filePath, iface, std::strerror(errno));
}

void ErrorGetDeviceIp(std::string filePath, std::string iface) {
    std::print("{}: No se pudo obtener la dirección IP de este dispocitivo en la interfaz {}\n"
               "GetDeviceIP: {}\n",
               filePath, iface, std::strerror(errno));
}

void ErrorStringToIp(std::string filePath, std::string str) {
    std::print("{}: Error al convertir la cadena {} en un formato de dirección IP\n"
               "StringToIP: {}\n",
               filePath, str, std::strerror(errno));
}

void ErrorSetDeviceMac(std::string filePath, std::string iface) {
    std::print("{}: Error al cambiar la dirección MAC en la interfaz {}\n"
               "SetDeviceMac: {}\n",
               filePath, iface, std::strerror(errno));
}
