// =====================================================================================
// 
//       Filename:  EventLoopThread.h
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年02月14日 19时43分35秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Gaoyuan, sishuiliunian0710@gmail.com
//        Company:  Class 1204 of Computer Science and Technology
// 
// =====================================================================================

#ifndef EVENTLOOPTHREAD_H
#define EVENTLOOPTHREAD_H

#include <boost/noncopyable.hpp>


#include "Condition.h"
#include "Mutex.h"
#include "Thread.h"


namespace liunian{
class EventLoop;

class EventLoopThread{
	public:
		EventLoopThread();
		~EventLoopThread();
		EventLoop *startLoop();

	private:
		void threadFunc();

		EventLoop *loop;
		bool quit;
		Thread thread;
		MutexLock mutex;
		Condtion condition;

};
}
#endif
