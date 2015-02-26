// =====================================================================================
// 
//       Filename:  Socket.h
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年02月17日 10时21分38秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Gaoyuan, sishuiliunian0710@gmail.com
//        Company:  Class 1204 of Computer Science and Technology
// 
// =====================================================================================

#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include "InetAddress.h"

namespace liunian{
class Socket{
	public:
		Socket()
		{
			socketFd = creatSocket();
		}
		Socket(int fd){
			socketFd = fd;
		}
		~Socket();
		int getSocketFd() const{
		//	std::cout << "get socket fd = " << socketFd << std::endl;
			return socketFd;
		}
		int creatSocket();
		
		void Bind(const InetAddress &);
		void Listen();
		int Accept(InetAddress &);
		void shutdownWrite();
		void setTcpNoDelay(bool on);
	private:
	 	int socketFd;
};
}
#endif
