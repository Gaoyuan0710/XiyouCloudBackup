// =====================================================================================
// 
//       Filename:  Acceptor.cpp
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年02月10日 17时31分30秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Gaoyuan, sishuiliunian0710@gmail.com
//        Company:  Class 1204 of Computer Science and Technology
// 
// =====================================================================================

#include <boost/bind.hpp>
#include <sys/socket.h>

#include "Acceptor.h"
#include "Socket.h"
#include "Channel.h"

using namespace liunian;
Acceptor::Acceptor(EventLoop *loop, 
			const InetAddress &listenAddr)
		:loop(loop),
		acceptSocket(),
//		acceptChannel(loop, 6),
		acceptChannel(loop, acceptSocket.getSocketFd()),
		listenFlag(false)
{
	acceptSocket.Bind(listenAddr);
	acceptChannel.setReadCallBack(boost::bind(&Acceptor::handleRead, this));
}
void Acceptor::listen(){
	listenFlag = true;
	acceptSocket.Listen();
	acceptChannel.enableReading();
}
void Acceptor::handleRead(){
	InetAddress clientAddr(0);

	int connfd = acceptSocket.Accept(clientAddr);

	if (connfd >= 0){

		std::cout << "establish " << std::endl;
		if (newConnectionCallBack){
			newConnectionCallBack(connfd, clientAddr);
		}
		else{
			close(connfd);
		}
	}
}

