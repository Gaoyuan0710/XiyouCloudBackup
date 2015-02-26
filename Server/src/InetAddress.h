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

#ifndef ADDRESSOPERATION_H
#define ADDRESSOPERATION_H

#include <string>
#include <netinet/in.h>

using std::string;

namespace liunian{
class InetAddress{
	public:
		explicit InetAddress(uint16_t port);

 		InetAddress(const string& ip, uint16_t port);
		
		InetAddress(const struct sockaddr_in& addr)
			: address(addr)
		{ }
		
		string showIpAndPort() const;
		const struct sockaddr_in getSockAddrInet() const { 
			return address; 
		}
		void setSockAddrInet(struct sockaddr_in& addr) { 
			address = addr; 
		}
	private:
		struct sockaddr_in address;
};
}
#endif
