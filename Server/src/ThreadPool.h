// =====================================================================================
// 
//       Filename:  ThreadPool.h
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年02月02日 19时15分05秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Gaoyuan, sishuiliunian0710@gmail.com
//        Company:  Class 1204 of Computer Science and Technology
// 
// =====================================================================================

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <string>
#include <queue>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include "Mutex.h"
#include "Condition.h"
#include "Thread.h"

using std::string;
using std::queue;

namespace liunian{
class ThreadPool : boost::noncopyable{
	public:
		typedef boost::function<void ()> Task;
		explicit ThreadPool(const string &name = string("ThreadPool"));
		~ThreadPool();
		void setMaxQueueSize(int maxSize){
			maxQueueSize = maxSize;
		}
		void setThreadWorkFunc(const Task &job){
			workToDo = job;
		}
		void start(int numThreads);
		void stop();
		void addWork(const Task &job);

	private:
		bool isFull() const;
		Task takeWork();
		void beginRunning();
		void pthreadPoolmanager();

		MutexLock mutex;
		Condtion notEmpty;
		Condtion notFull;
		string threadPoolName;
		Task workToDo;
		boost::ptr_vector<Thread> threads;
		queue<Task> workQueue;
		int maxQueueSize;
		bool isRunning;
};
}

#endif
