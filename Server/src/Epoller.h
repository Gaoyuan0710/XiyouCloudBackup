// =====================================================================================
// 
//       Filename:  Epoller.h
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年01月28日 00时11分58秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Gaoyuan, sishuiliunian0710@gmail.com
//        Company:  Class 1204 of Computer Science and Technology
// 
// =====================================================================================

#ifndef EPOLLER_H
#define EPOLLER_H

#include <iostream>
#include <sys/epoll.h>
#include <vector>
#include <map>

#include "EventLoop.h"
#include "Channel.h"
#include "Timestamp.h"

using std::vector;
using std::map;

namespace liunian{
class Epoll{
	public:
		typedef vector <struct epoll_event> EventList;
		typedef vector <Channel *> ChannelList;
		typedef map <int, Channel*> ChannelMap;
		
		Epoll(EventLoop *loop);
		~Epoll();
		Timestamp poll(int timeOut, 
					ChannelList *channelList);


		void updateChannel(Channel *channel);
		void removeChannel(Channel *channel);
		void fillActiveChannels(int numEvents, 
					ChannelList *channelList) const;
		void assertInLoopThread(){
					loop->assertInLoopThread();
				}
	private:
		int epollFd;
		EventList events;
		EventLoop *loop;
		ChannelMap channels;
};
}
#endif
