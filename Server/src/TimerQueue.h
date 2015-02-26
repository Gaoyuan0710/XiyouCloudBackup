// =====================================================================================
// 
//       Filename:  TimerQueue.h
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年02月12日 20时41分42秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Gaoyuan, sishuiliunian0710@gmail.com
//        Company:  Class 1204 of Computer Science and Technology
// 
// =====================================================================================

#ifndef TIMERQUEUE_H
#define TIMERQUEUE_H

#include <set>
#include <vector>

#include <boost/noncopyable.hpp>

#include "Timestamp.h"
#include "Mutex.h"
#include "Timer.h"
#include "Channel.h"
//#include "TimerId.h"

using std::pair;
using std::set;
using std::vector;

namespace liunian{
class EventLoop;
class Timer;
class TimerId;
	
class TimerQueue{
	public:
		TimerQueue(EventLoop *loop);
		~TimerQueue();
		TimerId addTimer(
					const TimerCallBack &cb,
					Timestamp when,
					double interval);

	private:
		typedef pair<Timestamp, Timer*> Entry;
		typedef set<Entry> TimerList;
		
		
		void addTimerInLoop(Timer *timer);
		void handlRead();

		vector<Entry> getEnpired(Timestamp now);
		void reset(const vector<Entry> &enpired,
					Timestamp now);
		bool insert(Timer *timer);

		EventLoop *loop;
		const int timeFd;
		Channel timeFdChannel;
		TimerList timers;


		int creatTimerFd();
		struct timespec howMuchTimeFromNow(
					Timestamp when);
		void readTimerFd(int timerFd, 
					Timestamp now);
		void resetTimerFd(int timerFd,
					Timestamp now);
};
}
#endif
