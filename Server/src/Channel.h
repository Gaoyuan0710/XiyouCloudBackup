// =====================================================================================
// 
//       Filename:  Channel.h
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年01月27日 22时49分08秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Gaoyuan, sishuiliunian0710@gmail.com
//        Company:  Class 1204 of Computer Science and Technology
// 
// =====================================================================================

#ifndef CHANNEL_H 
#define CHANNEL_H

#include <iostream>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>

//#include "EventLoop.h"

#include "Timestamp.h"

namespace liunian{
class EventLoop;

class Channel : boost::noncopyable{
	public:
		typedef boost::function<void ()> EventCallBack;
		typedef boost::function<void (Timestamp)> ReadEventCallBack;

		Channel(EventLoop *loop, int sockfd);
		~Channel();
		void handleEvent(Timestamp receiveTime);
		void setReadCallBack(const ReadEventCallBack &func){
			readCallBack = func;
		}
		void setWriteCallBack(const EventCallBack &func){
			writeCallBack = func;
		}
		void setErrorCallBack(const EventCallBack &func){
			errorCallBack = func;
		}
		void setCloseCallBack(const EventCallBack &func){
			closeCallBack = func;
		}
		void setRevents(int revent);
		void setIndex(int index);
		void enableReading();
		void enableWriting();
		void disableWriting();
		void disableAll();
		bool isWriting();
		int getEvents();
		int getSocket();
		int getIndex();
		EventLoop *getOwnLoop(){
			return loop;
		}
		bool isNoEvent() const{
			return events == 0;
		}

	private:
		void update();
		int index;
		int socketfd;
		int events;
		int revents;
		bool isInHandling;
		EventLoop *loop;
		EventCallBack writeCallBack;
		ReadEventCallBack readCallBack;
		EventCallBack errorCallBack;
		EventCallBack closeCallBack;
};
}
#endif
