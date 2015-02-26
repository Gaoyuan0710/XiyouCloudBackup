// =====================================================================================
// 
//       Filename:  ThreadPool.cpp
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年02月02日 19时56分36秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Gaoyuan, sishuiliunian0710@gmail.com
//        Company:  Class 1204 of Computer Science and Technology
// 
// =====================================================================================

#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <stdio.h>

#include "ThreadPool.h"

using namespace liunian;
using std::cout;
using std::endl;

ThreadPool::ThreadPool(const string &name)
		:mutex(),
		notEmpty(mutex),
		notFull(mutex),
		threadPoolName(name),
		maxQueueSize(0),
		isRunning(false)
{

}
ThreadPool::~ThreadPool(){
	if (isRunning){
		stop();
	}
}
void ThreadPool::start(int numThreads){
	isRunning = true;

	threads.reserve(numThreads);

	boost::function<void ()> f;
	f = boost::bind(&ThreadPool::pthreadPoolmanager, this);
	Thread managerThread(f, "managerThread");
	managerThread.start();

	for (int i = 0; i < numThreads; i++){
		char id[32];
		snprintf(id, sizeof(id), "%d", i + 1);
		threads.push_back(new Thread(boost::bind(&ThreadPool::beginRunning, this), threadPoolName +
						id));
		threads[i].start();
	}
	if (numThreads == 0 && workToDo){
		workToDo();
	}
}
void ThreadPool::stop(){
	{
		MutexLockGuard lock(mutex);
		isRunning = false;
		notEmpty.notigyAll();
	}
	for_each(threads.begin(), threads.end(), boost::bind(&Thread::join, _1));
}
void ThreadPool::addWork(const Task &task){
	if (threads.empty()){
		task();
	}
	else{
		MutexLockGuard lock(mutex);
		while (isFull()){
			notFull.wait();
		}
		workQueue.push(task);
		notEmpty.notigyAll();
	}
}

ThreadPool::Task ThreadPool::takeWork(){
	MutexLockGuard lock(mutex);

	while (workQueue.empty() && isRunning){
		notEmpty.wait();
	}
	Task task;
	if (!workQueue.empty()){
		task = workQueue.front();
		workQueue.pop();

		
		if (maxQueueSize > 0){
			notFull.notigyAll();
		}
	}
	return task;
}

bool ThreadPool::isFull() const{
	return maxQueueSize > 0 && workQueue.size() >= maxQueueSize;
}

void ThreadPool::beginRunning(){
	if (workToDo){
		workToDo();
	}

	while (isRunning){
		Task task(takeWork());

		if (task){
			task();
		}

		int work_size = workQueue.size();
		int pthread_num = threads.size();

	
		if (pthread_num / 3 > work_size){
			cout << "Too Many Pthreads" << endl;

			//exit thread;
		}
	}


}

void ThreadPool::pthreadPoolmanager(){
	while (isRunning){
		//sleep(5);

		int work_size = workQueue.size();
		int pthread_num = threads.size();

		cout << "Manager coming " << endl;
		cout << "Thread Num is " << pthread_num << " work size is "<< work_size << endl;

		if (pthread_num < work_size / 3){
			cout << "Too Many Works To Do" << endl;
			
			for (int i = pthread_num; i < work_size / 3; i++){
				char id[32];
				snprintf(id, sizeof(id), "%d", i + 1);
				threads.push_back(new Thread(boost::bind(&ThreadPool::beginRunning, this),
								threadPoolName + id));
				threads[i].start();
			}

		}
		else if (pthread_num / 3 > work_size){
			cout << "Too Many Pthreads" << endl;

			int thread_need_exit = pthread_num / 3 - work_size;

			cout << "Before :" << threads.size() << endl;
			for (int i = 0; i < thread_need_exit; i++){
				notEmpty.notify();
			}
			cout << "After :" << threads.size() << endl;
		}

	}
}
