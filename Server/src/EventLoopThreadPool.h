// =====================================================================================
// 
//       Filename:  EventLoopThreadPool.h
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年02月23日 17时42分40秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Gaoyuan, sishuiliunian0710@gmail.com
//        Company:  Class 1204 of Computer Science and Technology
// 
// =====================================================================================

#ifndef EVENTLOOPTHREADPOOL_H
#define EVENTLOOPTHREADPOOL_H

#include "Mutex.h"
#include "Condition.h"
#include "Thread.h"
#include "EventLoop.h"
#include "EventLoopThread.h"

#include <vector>
#include <boost/function.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace liunian{
class EventLoopThreadPool{
	public:
		EventLoopThreadPool(EventLoop *baseLoop);
		~EventLoopThreadPool();
		void setThreadNum(int nums){
			threadNums = nums;
		}
		void start();
		EventLoop *getNextLoop();
	private:
		EventLoop *baseLoop;
		bool ifStart;
		int threadNums;
		int next;
		boost::ptr_vector<EventLoopThread> threads;
		std::vector<EventLoop *> loops;


};
}

#endif
