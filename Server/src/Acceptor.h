// =====================================================================================
// 
//       Filename:  Acceptor.h
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年02月10日 17时22分37秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Gaoyuan, sishuiliunian0710@gmail.com
//        Company:  Class 1204 of Computer Science and Technology
// 
// =====================================================================================

#ifndef ACCEPTOR_H
#define ACCEPTOR_H


#include <boost/function.hpp>

#include "InetAddress.h"
#include "Socket.h"
#include "Channel.h"

namespace liunian{
class EventLoop;
class Channel;
class Acceptor{
	public:
		typedef boost::function<void (int sockfd, const InetAddress &)> DeadlNewConnectionCallBack;
		Acceptor(EventLoop *loop, const InetAddress &);

		void setDealNewConnectionCallBack(const DeadlNewConnectionCallBack &cb){
			newConnectionCallBack = cb;
		}
		bool ifListenning()const{
			return listenFlag;
		}
		void listen();
	private:
		void handleRead();
		EventLoop *loop;
		Socket acceptSocket;
		Channel acceptChannel;
		DeadlNewConnectionCallBack newConnectionCallBack;
		bool listenFlag;


};
}

#endif
