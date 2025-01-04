#ifndef __ERRORS_H__
#define __ERRORS_H__

#include <string>

void ErrorArgumentNotMatching(std::string, std::string, std::string);
void ErrorIfaceNotExist(std::string, std::string);
void ErrorInvalidMacAddress(std::string, std::string);
void ErrorInvalidIpAddress(std::string, std::string);
void ErrorSocketRaw(std::string, std::string);
void ErrorSendTo(std::string, std::string);
void ErrorRecvFrom(std::string, std::string);
void ErrorSockConfigTimeOut(std::string, std::string);
void ErrorGetDeviceMac(std::string, std::string);
void ErrorGetDeviceIp(std::string, std::string);
void ErrorStringToIp(std::string, std::string);
void ErrorSetDeviceMac(std::string, std::string);

#endif // __ERRORS_H__
