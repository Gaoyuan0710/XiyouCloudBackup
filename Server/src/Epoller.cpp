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

#include <iostream>
#include <vector>
#include <sys/epoll.h>
#include <errno.h>
#include <boost/static_assert.hpp>

#include "Epoller.h"
#include "Timestamp.h"

using namespace liunian;
using std::vector;
using std::endl;
using std::cout;

Epoll::Epoll(EventLoop *loop)
		:loop(loop),
		events(16)
	
{
	epollFd = epoll_create1(EPOLL_CLOEXEC);

	if (epollFd < 0){
		cout << "epoll_create error" << endl;
	}
}
Epoll::~Epoll(){
	close(epollFd);
}
Timestamp Epoll::poll(int timeOut,
			vector <Channel *> *channel){

//	std::cout << "Epoll::poll begin " << endl;
	//std::cout << "event size " << events.size() << std::endl;
	int fds = epoll_wait(epollFd, 
				&*events.begin(), 
				static_cast<int>(events.size()), 
				timeOut);

	Timestamp now(Timestamp::now());
	if (fds == -1){
		cout << "epoll_wait error" << errno
			<< " " << strerror(errno) 
			<<  endl;
//		return ;
	}
	if (fds > 0){
		fillActiveChannels(fds, channel);
	}
		
//	std::cout << "Epoll::poll end" << endl;


	return now;
}
void Epoll::fillActiveChannels(int numEvents,
			ChannelList *activeChannels) const{


	//assert(implicit_cast<size_t>(numEvents) <= events.size());


//	std::cout << "fillActiveChannels " << numEvents << std::endl;
	for (int i = 0; i < numEvents; i++){
		Channel *channelTemp = static_cast<Channel *>(events[i].data.ptr);

		int fd = channelTemp->getSocket();
		ChannelMap::const_iterator it = channels.find(fd);
		assert(it != channels.end());
		assert(it->second == channelTemp);


		channelTemp->setRevents(events[i].events);
		activeChannels->push_back(channelTemp);
	}
}
void Epoll::updateChannel(Channel *channel){
	assertInLoopThread();

	struct epoll_event temp;
	bzero(&temp, sizeof(temp));
	temp.data.ptr = channel;
	temp.events = channel->getEvents();
	
	const int index = channel->getIndex();

	if (index == -1 || index == 2){

		int fd = channel->getSocket();

		if (index == -1){
			assert(channels.find(fd) == channels.end());
			channels[fd] = channel;
		}
		else{
			assert(channels.find(fd) != channels.end() );
			assert(channels[fd] == channel);

		//	epoll_ctl(epollFd, EPOLL_CTL_MOD, fd, &temp);
		}
		channel->setIndex(1);
		epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &temp);

	}
	else{
		int fd = channel->getSocket();

		assert(channels.find(fd) != channels.end() );
		assert(channels[fd] == channel);
		assert(index == 1);
		if (channel->isNoEvent()){
			epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, &temp);
			channel->setIndex(2);
		}
		else{
			epoll_ctl(epollFd, EPOLL_CTL_MOD, fd, &temp);
		}
	}
}
void Epoll::removeChannel(Channel *channel){
	assertInLoopThread();

	int fd = channel->getSocket();

	assert(channels.find(fd) != channels.end());
	assert(channels[fd] == channel);
	//assert(channel->isNoEvent());


	int index = channel->getIndex();

	assert(index == 1 || index == 2);
//	std::cout << "remove Channel befor " << channels.size() << std::endl;
	size_t n = channels.erase(fd);

	assert(n == 1);
//	std::cout << "remove Channel after " << channels.size() << std::endl;
	struct epoll_event temp;

	temp.data.ptr = channel;
	temp.events = channel->getEvents();

	if (index == 1){
		epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, &temp);
	}

	channel->setIndex(-1);
}
