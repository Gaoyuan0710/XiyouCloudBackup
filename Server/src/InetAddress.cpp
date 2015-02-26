// =====================================================================================
// 
//       Filename:  AddressOperation.h
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年02月17日 09时55分16秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Gaoyuan, sishuiliunian0710@gmail.com
//        Company:  Class 1204 of Computer Science and Technology
// 
// =====================================================================================

#include <iostream>
#include <string>
#include <netinet/in.h>
#include <strings.h>
#include <arpa/inet.h>
#include <stdio.h>

#include "InetAddress.h"


using std::string;
using namespace liunian;

InetAddress::InetAddress(uint16_t port){
	bzero(&address, sizeof(address));

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(port);
}
InetAddress::InetAddress(const string &ip, uint16_t port){
	bzero(&address, sizeof(address));

	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	inet_pton(AF_INET, ip.c_str(), &address.sin_addr);

}
string InetAddress::showIpAndPort() const{
	char buf[32];
	char host[INET_ADDRSTRLEN] = "INVALID";
	
	inet_ntop(AF_INET, &address.sin_addr, host, sizeof(host));

	uint16_t port = ntohs(address.sin_port);
	snprintf(buf, sizeof(buf), "%s:%u", host, port);

	return buf;
}
