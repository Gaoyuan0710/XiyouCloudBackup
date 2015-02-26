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
#ifndef TIMERID_H
#define TIMERID_H

namespace liunian{
class Timer;

class TimerId {
	public:
		explicit TimerId(Timer* timer)
			: value(timer)
		{
		}
	private:
		Timer* value;
};

}

#endif  
