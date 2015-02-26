// =====================================================================================
// 
//       Filename:  TcpServer.cpp
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年02月17日 16时32分27秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Gaoyuan, sishuiliunian0710@gmail.com
//        Company:  Class 1204 of Computer Science and Technology
// 
// =====================================================================================
#include <stdio.h>
#include <boost/bind.hpp>
#include <boost/scoped_ptr.hpp>

#include "TcpServer.h"
#include "InetAddress.h"
#include "Acceptor.h"
#include "EventLoop.h"
#include "EventLoopThreadPool.h"

using namespace liunian;

TcpServer::TcpServer(EventLoop *loop, const InetAddress &addr):
	loop(loop),
	name(addr.showIpAndPort()),
	acceptor(new Acceptor(loop, addr)),
	threadPool(new EventLoopThreadPool(loop)),
	startFlag(false),
	nextConnectId(1)
{
	acceptor->setDealNewConnectionCallBack(
				boost::bind(&TcpServer::newConnection, this, _1, _2));
}

TcpServer::~TcpServer(){

}
void TcpServer::setThreadNum(int nums){
	threadPool->setThreadNum(nums);
}
void TcpServer::start(){
	if (!startFlag){
		startFlag = true;
		threadPool->start();
	}

	if (!acceptor->ifListenning()){
		loop->runInLoop(boost::bind(&Acceptor::listen, get_pointer(acceptor)));
	}
}
void TcpServer::newConnection(int socketFd, const InetAddress &addr){
	char buf[32];

	snprintf(buf, sizeof(buf), "#%d", nextConnectId);
	++nextConnectId;

	std::string connectionName = name + buf;

	struct sockaddr_in localAddr;
	bzero(&localAddr, sizeof(localAddr));
	socklen_t len = sizeof(localAddr);

	getsockname(socketFd, (struct sockaddr*)&localAddr, &len);

	InetAddress loaclAddr(localAddr);

	EventLoop *ioLoop = threadPool->getNextLoop();

	std::cout << "new connection come" << std::endl;
	
	TcpConnectionPtr newConnect(new TcpConnection(ioLoop, connectionName, socketFd, localAddr, addr));
	connections[connectionName] = newConnect;

	newConnect->setConnectionCallBack(connectionCallBack);
	newConnect->setMessageCallBack(messageCallBack);
	newConnect->setWriteCompleteCallBack(writeCompleteCallBack);
	newConnect->setCloseCallBack(
				boost::bind(&TcpServer::removeConnection, this, _1));

	
	ioLoop->runInLoop(boost::bind(&TcpConnection::connectionEstablished, newConnect));
}

void TcpServer::removeConnection(const TcpConnectionPtr &connect){
	loop->runInLoop(boost::bind(&TcpServer::removeConnectionInLoop, this, connect));
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr & connect){
	
	loop->assertInLoopThread();

	std::cout << " TcpServer removeConnectionInLoop" << std::endl;
//	sleep(10);
	size_t n = connections.erase(connect->getname());

	assert(n == 1);

	EventLoop *ioLoop = connect->getLoop();

	ioLoop->queueInLoop(
				boost::bind(&TcpConnection::connectionDestroyed, connect));

}

