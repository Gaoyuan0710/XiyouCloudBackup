// =====================================================================================
// 
//       Filename:  EventLoop.h
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年01月27日 22时14分25秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Gaoyuan, sishuiliunian0710@gmail.com
//        Company:  Class 1204 of Computer Science and Technology
// 
// =====================================================================================

#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <iostream>
#include <pthread.h>
#include <boost/noncopyable.hpp>
#include <sys/types.h>
#include <boost/scoped_ptr.hpp>
#include <vector>

#include "Timestamp.h"
#include "Thread.h"
#include "Mutex.h"
#include "TimerId.h"
#include "Timer.h"
#include "TimerQueue.h"


using std::vector;

namespace liunian{
class Channel;
class Epoll;
class TimerQueue;
class Timestamp;

class EventLoop : boost::noncopyable{
	public:
		typedef boost::function<void ()> Functor;
		EventLoop();
		~EventLoop();
		void loop();
		void quit();
		void wakeup();
		bool isInLoopThread();
		

		void updateChannel(Channel *channel);
		void removeChannel(Channel *channel);
		void assertInLoopThread(){
			if (!isInLoopThread()){
				abortNotInLoopThread();
			}
		
		}
		void runInLoop(const Functor &cb);
		void queueInLoop(const Functor &cb);
		TimerId runAt(const Timestamp &time,
					const Functor &cb);
		TimerId runAfter(double delay,
					const Functor &cb);
		TimerId runEvery(double interval,
					const Functor &cb);
		Timestamp getPollRetrunTime(){
			return pollReturnTime;
		}
	private:

		void handleRead();
		void doPendingFunctors();

		typedef vector<Channel *> ChannelList;
		ChannelList activeChannels;
		bool loopFlag;
		const pid_t threadId;
		boost::scoped_ptr<Epoll> epoll;
//		Epoll *epoll;
		void abortNotInLoopThread();
		
		Timestamp pollReturnTime;
		
		boost::scoped_ptr<TimerQueue> timerQueue;
		int wakeUpFd;
		boost::scoped_ptr<Channel> wakeupChannel;
		MutexLock mutex;
		vector<Functor> pendingFunctors;
		bool callingPendingFunctors;
};
}
#endif
