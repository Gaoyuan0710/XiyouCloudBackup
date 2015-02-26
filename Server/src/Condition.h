// =====================================================================================
// 
//       Filename:  Condition.h
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年01月31日 14时04分55秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Gaoyuan, sishuiliunian0710@gmail.com
//        Company:  Class 1204 of Computer Science and Technology
// 
// =====================================================================================

#ifndef CONDITION_H
#define CONDITION_H

#include <boost/noncopyable.hpp>
#include "Mutex.h"

namespace liunian{
class Condtion :boost::noncopyable {
	public:
		explicit Condtion(MutexLock &mutex) : mutex_(mutex){
			pthread_cond_init(&cond, NULL);
		}
		~Condtion(){
			pthread_cond_destroy(&cond);
		}
		void wait(){
			pthread_cond_wait(&cond, mutex_.getMutex());
		}
		bool notify(){
			return pthread_cond_signal(&cond) == 0;
		}
		bool notigyAll(){
			return pthread_cond_broadcast(&cond) == 0;
		}
	private:
		MutexLock &mutex_;
		pthread_cond_t cond;
};
}
#endif
