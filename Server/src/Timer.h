// =====================================================================================
// 
//       Filename:  Timer.h
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年02月12日 19时39分44秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Gaoyuan, sishuiliunian0710@gmail.com
//        Company:  Class 1204 of Computer Science and Technology
// 
// =====================================================================================
#ifndef TIMER_H
#define TIMER_H

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include "Timestamp.h"

namespace liunian{
class Timestamp;

typedef boost::function<void ()> TimerCallBack;

class Timer{
	public:
		Timer(const TimerCallBack &cb,
					Timestamp when,
					double interval);
		void run() const;
		Timestamp getExpiration() const;
		bool ifRepeat() const;
		void restart(Timestamp now);
	private:
		const TimerCallBack callBack;
		Timestamp 	expiration;
		const double interval;
		const bool repeat;
};
}
#endif
