// =====================================================================================
// 
//       Filename:  TcpConnection.h
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年02月17日 17时32分59秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Gaoyuan, sishuiliunian0710@gmail.com
//        Company:  Class 1204 of Computer Science and Technology
// 
// =====================================================================================

#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include "InetAddress.h"
#include "TcpServer.h"
#include "CallBack.h"
#include "Timestamp.h"
#include "Buffer.h"

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <string>

using std::string;

namespace liunian{
class Channel;
class EventLoop;
class Socket;

class TcpConnection : public boost::enable_shared_from_this<TcpConnection>{
	public:
		TcpConnection(EventLoop *loop,
					const std::string &name,
					int sockfd,
					const InetAddress &loaclAddr,
					const InetAddress &otherAddr);
		~TcpConnection();
		EventLoop *getLoop()const{
			return loop;
		}
		const string&getname() const{
			return name;
		}
		const InetAddress &getLoaclAddress(){
			return localAddr;
		}
		const InetAddress &getOtherAddress(){
			return otherAddr;
		}
		bool ifEstablish() const{
			return state == kEstablish;
		}
		void setConnectionCallBack(const ConnectionCallBack &cb){
			connectionCallBack = cb;
		}
		void setMessageCallBack(const MessageCallBack &cb){
			messageCallBack = cb;
		}
		void setCloseCallBack(const CloseCallBack& cb){
			closeCallBack = cb;
		}
		void setWriteCompleteCallBack(const WriteCompleteCallBack &cb){
			writeCompleteCallBack = cb;
		}
		void send(const std::string &);
		void shutdown();
		void connectionEstablished();
		void connectionDestroyed();
		int getState(){
			return state;
		}
	private:
		enum State{
			kConnecting,
			kEstablish,
			kDisConnectiong,
			kDisConnected,
		};
		void setState(State s){
			state = s;	
		}
		void handleRead(Timestamp recvTime);
		void handleWrite();
		void handleClose();
		void handleError();

		void sendInLoop(const std::string &);
		void shutdownInLoop();
		void setTcpNoDelay(bool);
//		void shutdown();
		EventLoop *loop;
		string name;
		State state;

		boost::scoped_ptr<Socket> socket;
		boost::scoped_ptr<Channel> channel;

		InetAddress localAddr;
		InetAddress otherAddr;
		ConnectionCallBack connectionCallBack;
		MessageCallBack messageCallBack;
		WriteCompleteCallBack writeCompleteCallBack;
		CloseCallBack closeCallBack;
		Buffer inputBuffer;
		Buffer outputBuffer;
};
}
#endif
