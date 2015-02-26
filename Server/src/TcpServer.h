// =====================================================================================
// 
//       Filename:  TcpServer.h
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年02月17日 16时32分14秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Gaoyuan, sishuiliunian0710@gmail.com
//        Company:  Class 1204 of Computer Science and Technology
// 
// =====================================================================================

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <map>
#include <string>

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include "TcpConnection.h"
#include "InetAddress.h"
#include "CallBack.h"

using std::map;
using std::string;

namespace liunian{
class Acceptor;
class EventLoop;
class EventLoopThreadPool;

class TcpServer{
	public:
		TcpServer(EventLoop *loop,  const InetAddress &addr);
		~TcpServer();

		void setThreadNum(int nums);

		void start();
		void setConnectionCallBack(const ConnectionCallBack &cb){
			connectionCallBack = cb;
		}
		void setMessageCallBack(const MessageCallBack &cb){
			messageCallBack = cb;
		}
		void setWriteCompleteCallBack(const WriteCompleteCallBack &cb){
			writeCompleteCallBack = cb;
		}
	private:
		EventLoop *loop;
		string name;
		void newConnection(int fd, const InetAddress &addr);
		void removeConnection(const TcpConnectionPtr&);
		boost::scoped_ptr<Acceptor> acceptor;
		boost::scoped_ptr<EventLoopThreadPool> threadPool;
		void removeConnectionInLoop(const TcpConnectionPtr &);
		ConnectionCallBack connectionCallBack;
		MessageCallBack messageCallBack;
		WriteCompleteCallBack writeCompleteCallBack;

		bool startFlag;
		int nextConnectId;
		ConnectionMap  connections;		
};
}

#endif
