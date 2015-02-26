// =====================================================================================
// 
//       Filename:  TimerQueue.cpp
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年02月12日 21时08分32秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Gaoyuan, sishuiliunian0710@gmail.com
//        Company:  Class 1204 of Computer Science and Technology
// 
// =====================================================================================
#include "TimerQueue.h"
#include "EventLoop.h"
#include "TimerId.h"

#include <boost/bind.hpp>
#include <sys/timerfd.h>

using namespace liunian;
using std::cout;
using std::endl;

int TimerQueue::creatTimerFd(){
	int timerfd = timerfd_create(CLOCK_MONOTONIC,
				TFD_NONBLOCK | TFD_CLOEXEC);

	if (timerfd < 0){
		cout << "Failed in timer_creadte" << endl;
	}

	return timerfd;
}

struct timespec TimerQueue::howMuchTimeFromNow(
			Timestamp when){
	int64_t microseconds = 
		when.getMircoSecondsSinceEpoch()
		- Timestamp::now().getMircoSecondsSinceEpoch();

	if (microseconds < 100){
		microseconds = 100;
	}

	struct timespec ts;
	
	ts.tv_sec = static_cast<time_t>(microseconds/Timestamp::kMincroSecondsPerSecond);
	
	ts.tv_nsec = static_cast<long> (
				(microseconds % 
				 Timestamp::kMincroSecondsPerSecond) 
				* 1000);
	return ts;
}

void TimerQueue::readTimerFd(int timerFd, Timestamp now){
	uint64_t howmany;
	ssize_t n = read(timerFd, &howmany, sizeof(howmany));

//	cout << "TimerQueue::handleRead() " << howmany
//		<< " at " << now.toString() << endl; 
	if (n != sizeof(howmany)){
		cout << "TimerQueue::handleRead() reads " 
			<< n << " bytes instead of 8" << endl;
	}
}

void TimerQueue::resetTimerFd(int timeFd, 
			Timestamp expiration){
	struct itimerspec newValue;
	struct itimerspec oldValue;

	bzero(&newValue, sizeof(newValue));
	bzero(&oldValue, sizeof(oldValue));

	newValue.it_value = howMuchTimeFromNow(expiration);

	int ret = timerfd_settime(timeFd, 0,
				&newValue, &oldValue);

	if (ret) {
		cout << "timer_settime error" << endl;
	}
}

TimerQueue::TimerQueue(EventLoop *loop)
		:loop(loop),
		timeFd(creatTimerFd()),
		timeFdChannel(loop, timeFd),
		timers()
{
	timeFdChannel.setReadCallBack(
				boost::bind(&TimerQueue::handlRead, this));

	timeFdChannel.enableReading();
}

TimerQueue::~TimerQueue(){
	close(timeFd);
	for (TimerList::iterator it = timers.begin();
				it != timers.end();
				it++){
		delete it->second;
	}
}

TimerId TimerQueue::addTimer(const TimerCallBack &cb,
			Timestamp when,
			double interval){
	Timer *timer = new Timer(cb, when, interval);

	loop->runInLoop(
			boost::bind(&TimerQueue::addTimerInLoop, 
				this, timer));
	return TimerId(timer);
}

void TimerQueue::addTimerInLoop(Timer *timer){
	loop->assertInLoopThread();

	bool earliestChanged = insert(timer);

	if (earliestChanged){
		resetTimerFd(timeFd, timer->getExpiration());
	}
}
void TimerQueue::handlRead(){
	loop->assertInLoopThread();

	Timestamp now(Timestamp::now());
	readTimerFd(timeFd, now);

	vector<Entry> expired = getEnpired(now);
	for (vector<Entry>::iterator it = expired.begin();
				it != expired.end();
				it++){
		it->second->run();
	}

	reset(expired, now);
}

vector<TimerQueue::Entry> 
TimerQueue::getEnpired(Timestamp now){
	vector<Entry> expired;
	Entry sentry = std::make_pair(now,
				reinterpret_cast<Timer *>
				(UINTPTR_MAX));

	TimerList::iterator it = timers.lower_bound(sentry);

	std::copy(timers.begin(),
				it, std::back_inserter(expired));
	timers.erase(timers.begin(), it);

	return expired;
}

void TimerQueue::reset(const vector<Entry> &expired,
			Timestamp now){
	Timestamp nextExpired;


	for (vector<Entry>::const_iterator it = expired.begin();
				it != expired.end();
				it++){
		if (it->second->ifRepeat()){
			it->second->restart(now);
			insert(it->second);
		}
		else{
			delete it->second;
		}
	}
	if (!timers.empty()){
		nextExpired = timers.begin()->second->getExpiration();

	}
	if (nextExpired.valid()){
		resetTimerFd(timeFd, nextExpired);
	}
}

bool TimerQueue::insert(Timer *timer){
	bool earliestChanged = false;
	Timestamp when = timer->getExpiration();
	TimerList::iterator it = timers.begin();
	if (it == timers.end() || when < it->first){
		earliestChanged = true;
	}

  std::pair<TimerList::iterator, bool> result =
          timers.insert(std::make_pair(when, timer));
  return earliestChanged;
}
