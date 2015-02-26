// =====================================================================================
// 
//       Filename:  TcpConnection.cpp
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年02月17日 19时04分03秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Gaoyuan, sishuiliunian0710@gmail.com
//        Company:  Class 1204 of Computer Science and Technology
// 
// =====================================================================================

#include <boost/bind.hpp>
#include <iostream>

#include "TcpConnection.h"
#include "Socket.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Timestamp.h"

using namespace liunian;

TcpConnection::TcpConnection(
			EventLoop *loop,
			const std::string&name,
			int sockfd,
			const InetAddress& localAddr,
			const InetAddress& otherAddr)
		:loop(loop),
		name(name),
		state(kConnecting),
		socket(new Socket(sockfd)),
		channel(new Channel(loop, sockfd)),
		localAddr(localAddr),
		otherAddr(otherAddr){
			channel->setReadCallBack(
						boost::bind(&TcpConnection::handleRead, this, _1));
			channel->setWriteCallBack(
						boost::bind(&TcpConnection::handleWrite, this));
			channel->setCloseCallBack(
						boost::bind(&TcpConnection::handleClose, this));
			channel->setErrorCallBack(
						boost::bind(&TcpConnection::handleError, this));
		}
TcpConnection::~TcpConnection(){
	std::cout << "TcoConnection " << this << " fd = " << channel->getSocket() << std::endl;
}

void TcpConnection::connectionEstablished(){
	loop->assertInLoopThread();
	assert(state == kConnecting);
	setState(kEstablish);
	channel->enableReading();
	connectionCallBack(shared_from_this());
}
void TcpConnection::connectionDestroyed(){
	

	loop->assertInLoopThread();
	assert(state == kEstablish || state == kDisConnectiong);
	setState(kDisConnected);
	channel->disableAll();

	connectionCallBack(shared_from_this());
	loop->removeChannel(get_pointer(channel));

}
void TcpConnection::handleRead(Timestamp recvTime){
	char buf[65535];
	int saveError = 0;

	ssize_t n = inputBuffer.readFd(channel->getSocket(), &saveError);

	//std::cout << "n = " << n << std::endl;
	//sleep(10);
	if (n > 0){
		messageCallBack(shared_from_this(), &inputBuffer, recvTime);
	}
	else if (n == 0){
		std::cout << "TcpConnection handleRead should be closed to handleClose " << state << std::endl;
		//sleep(10);
		handleClose();
	}
	else {
		errno = saveError;
		handleError();
	}
}

void TcpConnection::handleWrite(){
	loop->assertInLoopThread();

	if (channel->isWriting()){
		ssize_t writeLen = write(channel->getSocket(), outputBuffer.readStartIndex(), outputBuffer.readableSize());

		if (writeLen > 0){
			outputBuffer.retrieve(writeLen);
			if (outputBuffer.readableSize() == 0){
				channel->disableWriting();

				if (writeCompleteCallBack){
					loop->queueInLoop(boost::bind(writeCompleteCallBack, shared_from_this()));
				}
				if (state == kDisConnectiong){
					shutdownInLoop();
				}
			}
		}
	}
}

void TcpConnection::handleClose(){
	loop->assertInLoopThread();
	std::cout << "TcpConnection handleClose state is " << state << std::endl;
	assert(state == kEstablish || state == kDisConnectiong);
	channel->disableAll();
	closeCallBack(shared_from_this());
}
void TcpConnection::handleError(){
	std::cout << "Tcp handleError" << std::endl;
}

void TcpConnection::send(const std::string &message){
	if (state == kEstablish){
		if (loop->isInLoopThread()){
			sendInLoop(message);
		}
		else{
			loop->runInLoop(
						boost::bind(&TcpConnection::sendInLoop, this, message));
		}
	}
}
void TcpConnection::sendInLoop(const std::string &message){
	loop->assertInLoopThread();

	ssize_t writeLen = 0;

	if (!channel->isWriting() && outputBuffer.readableSize() == 0){
		writeLen = write(channel->getSocket(), message.data(), message.size());
		if (writeLen >= 0){
			if (implicit_cast<size_t>(writeLen) < message.size()){
				outputBuffer.append(message.data() + writeLen, message.size() - writeLen);
				if (!channel->isWriting()){
					channel->enableWriting();
				}
			}
			else if (writeCompleteCallBack){
				loop->queueInLoop(boost::bind(writeCompleteCallBack, shared_from_this()));
			}
			else{
				writeLen = 0;
			}
		}
	}
}
void TcpConnection::shutdown(){
	if (state == kEstablish){
		setState(kDisConnectiong);
		loop->runInLoop(
					boost::bind(&TcpConnection::shutdownInLoop, this));
	}
}
void TcpConnection::shutdownInLoop(){
	loop->assertInLoopThread();

	if (!channel->isWriting()){
		socket->shutdownWrite();
	}
}
void TcpConnection::setTcpNoDelay(bool on){
	socket->setTcpNoDelay(on);
}
