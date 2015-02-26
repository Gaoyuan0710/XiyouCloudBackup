// =====================================================================================
// 
//       Filename:  EventLoopThread.cpp
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年02月14日 19时50分42秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Gaoyuan, sishuiliunian0710@gmail.com
//        Company:  Class 1204 of Computer Science and Technology
// 
// =====================================================================================

#include "EventLoopThread.h"
#include "EventLoop.h"

#include <boost/bind.hpp>

using namespace liunian;

EventLoopThread::EventLoopThread()
		:loop(NULL),
		quit(false),
		thread(boost::bind(&EventLoopThread::threadFunc, this)),
		mutex(),
		condition(mutex)
{}
EventLoopThread::~EventLoopThread()
{
	quit = true;
	loop->quit();
	thread.join();
}

EventLoop * EventLoopThread::startLoop(){
	thread.start();
	{
		MutexLockGuard lock(mutex);
		while(loop == NULL){
			condition.wait();
		}
	}
	return loop;
}

void EventLoopThread::threadFunc(){
	EventLoop loop_;
	{
		MutexLockGuard lock(mutex);
		loop = &loop_;
		condition.notify();
	}
	loop_.loop();
}
