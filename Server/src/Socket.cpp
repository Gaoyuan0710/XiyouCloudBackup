// =====================================================================================
// 
//       Filename:  Socket.cpp
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年02月17日 10时52分22秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Gaoyuan, sishuiliunian0710@gmail.com
//        Company:  Class 1204 of Computer Science and Technology
// 
// =====================================================================================
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <iostream>
#include <unistd.h>
#include <netinet/tcp.h>

#include "Socket.h"
#include "InetAddress.h"

using namespace liunian;

using std::cout;
using std::endl;

Socket::~Socket(){
	close(socketFd);
}
void Socket::Bind(const InetAddress &addr){
	struct sockaddr_in tmpAddr = 
		addr.getSockAddrInet();

	int ret  = bind(socketFd, (struct sockaddr *)&tmpAddr, sizeof(tmpAddr));

	
	if (ret < 0){
		cout << "socket bind error" << endl;
	}
}
void Socket::Listen(){
	int ret = listen(socketFd, 5);

	if (ret < 0){
		cout << "socket listen error" << endl;
	}
}
int Socket::Accept(InetAddress &addr){
	struct sockaddr_in clientAddr;
	socklen_t len = sizeof(clientAddr);

	bzero(&clientAddr, sizeof(clientAddr));

	int connFd = accept(socketFd, (sockaddr *)&clientAddr, &len);

	if (connFd < 0){
		cout << "accept error" << endl;
	}
	if (connFd >= 0){
		cout << "new connect"  << endl;
		addr.setSockAddrInet(clientAddr);
	}

	return connFd;
}
int Socket::creatSocket(){
	int ret = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ret < 0){
		cout << "socket create error"<< endl;
	}

	cout << "socketFd = " << ret << endl;
	return ret;
}
void Socket::shutdownWrite(){
	if (shutdown(socketFd, SHUT_WR) < 0){
		cout << "shutdown write error" << endl;
	}
}
void Socket::setTcpNoDelay(bool on){
	int optval = on ? 1 : 0;

	setsockopt(socketFd, IPPROTO_TCP, TCP_NODELAY,
				&optval, sizeof optval);
}
