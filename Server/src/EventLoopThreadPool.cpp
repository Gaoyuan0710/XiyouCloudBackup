// =====================================================================================
// 
//       Filename:  EventLoopThreadPool.cpp
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年02月23日 18时46分26秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Gaoyuan, sishuiliunian0710@gmail.com
//        Company:  Class 1204 of Computer Science and Technology
// 
// =====================================================================================

#include "EventLoopThreadPool.h"
#include "EventLoopThread.h"
#include "EventLoop.h"

#include <boost/bind.hpp>

using namespace liunian;

EventLoopThreadPool::EventLoopThreadPool(EventLoop *baseLoop):
	baseLoop(baseLoop),
	ifStart(false),
	threadNums(0),
	next(0){
		
	}
EventLoopThreadPool::~EventLoopThreadPool(){
	
}
void EventLoopThreadPool::start(){
	baseLoop->assertInLoopThread();
	ifStart = true;

	for (int i = 0; i < threadNums; i++){
		EventLoopThread *t = new EventLoopThread;
		threads.push_back(t);
		loops.push_back(t->startLoop());
	}
}
EventLoop* EventLoopThreadPool::getNextLoop(){
	baseLoop->assertInLoopThread();
	EventLoop *loop = baseLoop;

	
	if (!loops.empty()){
		loop = loops[next];
		std::cout << "get next loop is " << next << std::endl;
		++next;
		if (static_cast<size_t>(next) >= loops.size()){
			next = 0;
		}
	}
	return loop;
}
