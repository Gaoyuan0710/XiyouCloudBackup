// =====================================================================================
// 
//       Filename:  Timer.cpp
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年02月12日 20时28分58秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Gaoyuan, sishuiliunian0710@gmail.com
//        Company:  Class 1204 of Computer Science and Technology
// 
// =====================================================================================
#include "Timer.h"
#include "Timestamp.h"


using namespace liunian;

Timer::Timer(const TimerCallBack &cb,
			Timestamp when,
			double interval)
		:callBack(cb),
		expiration(when),
		interval(interval),
		repeat(interval > 0.0)
{

}

void Timer::run() const{
	callBack();
}

Timestamp Timer::getExpiration() const{
	return expiration;
}

bool Timer::ifRepeat() const{
	return repeat;
}

void Timer::restart(Timestamp now){
	if (repeat){
		expiration = ::addTime(now, interval);
	}
	else{
//		std::cout << "ooooooooooo" << std::endl;
//		expiration = Timestamp();
	}
}
