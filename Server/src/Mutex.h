// =====================================================================================
// 
//       Filename:  Mutex.h
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年01月19日 15时28分42秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Gaoyuan, sishuiliunian0710@gmail.com
//        Company:  Class 1204 of Computer Science and Technology
// 
// =====================================================================================

#ifndef MUTEX_H
#define MUTEX_H

#include <iostream>
#include <pthread.h>
#include <boost/noncopyable.hpp>


namespace liunian {
class MutexLock : boost::noncopyable{
	private:
		pthread_mutex_t mutex;
	public:
		MutexLock(){
			if (pthread_mutex_init(&mutex, NULL) != 0){
				throw std::exception();
			}
		}
		~MutexLock(){
			pthread_mutex_destroy(&mutex);
		}
		bool lock(){
			return pthread_mutex_lock(&mutex) == 0;
		}
		bool unlock(){
			return pthread_mutex_unlock(&mutex) == 0;
		}
		pthread_mutex_t *getMutex(){
			return &mutex;
		}
};

class MutexLockGuard : boost::noncopyable{
	private:
		MutexLock &mutex;
	public:
		explicit MutexLockGuard(MutexLock &mutexLock) : mutex(mutexLock){
			mutex.lock();
		}
		~MutexLockGuard(){
			mutex.unlock();
		}
};
}

#endif
