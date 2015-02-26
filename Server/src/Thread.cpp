// =====================================================================================
// 
//       Filename:  Thread.cpp
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年02月02日 11时05分53秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Gaoyuan, sishuiliunian0710@gmail.com
//        Company:  Class 1204 of Computer Science and Technology
// 
// =====================================================================================

#include <iostream>
#include <unistd.h>

#include "Thread.h"

using namespace liunian;

Thread::Thread(const ThreadFunc &Func, const string &threadName) : 
	isStart(false), 
	isJoin(false), 
	func(Func), 
	threadName(threadName){
	;
}
Thread::~Thread(){
	if (isStart && !isJoin){
		pthread_detach(pthreadId);
	}
}
bool Thread::start(){
	isStart = true;
	return pthread_create(&pthreadId, NULL, &startThread, this) == 0;
}
void * Thread::startThread(void *obj){
	Thread * thread = static_cast<Thread *>(obj);
	thread->func();

	return NULL;
}
int Thread::join(pthread_t pthread){
	isJoin = true;

	return pthread_join(pthread, NULL);
}
int Thread::join(){
	isJoin = true;

	return pthread_join(pthreadId, NULL);
}

namespace CurrentThread{
	__thread int  t_cachedTid = 0;
}

pid_t Thread::gettid(){
	return static_cast<pid_t>(::syscall(SYS_gettid));
}
