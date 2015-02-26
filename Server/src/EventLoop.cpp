// =====================================================================================
// 
//       Filename:  EventLoop.cpp
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年01月27日 22时14分44秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Gaoyuan, sishuiliunian0710@gmail.com
//        Company:  Class 1204 of Computer Science and Technology
// 
// =====================================================================================

#include <iostream>
#include <assert.h>
#include <sys/eventfd.h>
#include <boost/bind.hpp>
#include <signal.h>

#include "EventLoop.h"
#include "Channel.h"
#include "Epoller.h"
#include "CurrentThread.h"
#include "Timestamp.h"

using namespace liunian;
using namespace CurrentThread;

using std::cout;
using std::endl;

__thread EventLoop* t_loopInThisThread = 0;


static int createEventfd()
{
  int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  if (evtfd < 0)
  {
      cout << "Failed in eventfd" << endl;
      abort();
    }
  return evtfd;
}

EventLoop::EventLoop()
		:loopFlag(false),
		epoll(new Epoll(this)),
		threadId(CurrentThread::tid()),
		callingPendingFunctors(false),
		timerQueue(new TimerQueue(this)),
		wakeUpFd(createEventfd()),
	//	pollReturnTime(0),
		wakeupChannel(new Channel(this, wakeUpFd))
{
//	cout << "EventLoop Create " << this << "in thread"
//		<< threadId << endl;

//	cout << "WakeUpFd = " << wakeUpFd << "Channel size = " << (wakeupChannel == NULL) << endl;
	if (t_loopInThisThread){
		cout << "Another EventLoop " 
			<< t_loopInThisThread 
			<< "Exits in this thread " << threadId 
			<< endl;
	}
	else{
		t_loopInThisThread = this;
	}

	wakeupChannel->setReadCallBack(boost::bind(&EventLoop::handleRead, this));
	wakeupChannel->enableReading();
	
}
EventLoop::~EventLoop(){
	close(wakeUpFd);
	t_loopInThisThread = NULL;
}
void EventLoop::loop(){

	assertInLoopThread();

	loopFlag = true;

	while (loopFlag){
		activeChannels.clear();
		int timeOut = -1;

		pollReturnTime = epoll->poll(timeOut, &activeChannels);



//		std::cout << "EventLoop loop activeChannels size " << activeChannels.size() << std::endl;
		for (ChannelList::iterator it =
					activeChannels.begin();
					it != activeChannels.end();
					it++){
			(*it)->handleEvent(pollReturnTime);
		}
		doPendingFunctors();
	}
}
//void EventLoop::quit(){
//	loopFlag = false;
//}
void EventLoop::updateChannel(Channel *channel){
	assert(channel->getOwnLoop() == this);
	assertInLoopThread();


	epoll->updateChannel(channel);
}
void EventLoop::removeChannel(Channel *channel){
	assert(channel->getOwnLoop() == this);
	assertInLoopThread();

	epoll->removeChannel(channel);
}
bool EventLoop::isInLoopThread(){
	return threadId == CurrentThread::tid();
} 

void EventLoop::abortNotInLoopThread(){
	cout << "EventLoop::abortNotInLoopThread - EventLoop " << this << " was created in threadId = "
		<< threadId << "  current thread id  = "
		<< CurrentThread::tid();
}



void EventLoop::quit()
{
  loopFlag = false;

  if (!isInLoopThread())
  {
      wakeup();
    }
}

void EventLoop::runInLoop(const Functor& cb)
{
  if (isInLoopThread())
  {
//	  std::cout << "EventLoop isInLoopThread" << std::endl;
	  //sleep(10);
      cb();
    }
  else
  {
//	  std::cout << "EventLoop isNotInLoopThread" << std::endl;
      queueInLoop(cb);
    }
}

void EventLoop::queueInLoop(const Functor& cb)
{
  {
    MutexLockGuard lock(mutex);
//	std::cout << pendingFunctors.size() << std::endl;
    pendingFunctors.push_back(cb);
	
//	std::cout << pendingFunctors.size() << std::endl;
    }

 // cout << "queueInLoop flag " << !isInLoopThread() << "  " << callingPendingFunctors << endl;
  if (!isInLoopThread() || callingPendingFunctors)
  {
      wakeup();
    }
}

TimerId EventLoop::runAt(const Timestamp& time, const Functor& cb)
{
  return timerQueue->addTimer(cb, time, 0.0);
}

TimerId EventLoop::runAfter(double delay, const Functor& cb)
{
  Timestamp time(addTime(Timestamp::now(), delay));
  return runAt(time, cb);
}

TimerId EventLoop::runEvery(double interval, const Functor& cb)
{
  Timestamp time(addTime(Timestamp::now(), interval));
  return timerQueue->addTimer(cb, time, interval);
}

void EventLoop::wakeup()
{
	uint64_t one = 1;
  	ssize_t n = ::write(wakeUpFd, &one, sizeof one);
  	
//	std::cout << "wake up " << std::endl;

	//std::cout << "Write " << n << " sizeof one " << sizeof one << std::endl;

	if (n != sizeof one)
	{
		std::cout << "EventLoop::wakeup() writes " << n << " bytes instead of 8";
    }
}



void EventLoop::doPendingFunctors()
{
  std::vector<Functor> functors;
  callingPendingFunctors = true;

  {
  MutexLockGuard lock(mutex);
  functors.swap(pendingFunctors);
  }

 
  if (functors.size() != 0)
	std::cout << "doPendingFunctors size " << functors.size() << std::endl;
 
  for (size_t i = 0; i < functors.size(); ++i)
  {
    functors[i]();
  }
  callingPendingFunctors = false;
}
void EventLoop::handleRead()
{
  uint64_t one = 1;
  ssize_t n = ::read(wakeUpFd, &one, sizeof one);
  if (n != sizeof one)
  {
	  std::cout << "EventLoop::handleRead() reads " << n << " bytes instead of 8";
    }
}



class IgnoreSigPipe{
	public:
		IgnoreSigPipe(){
			signal(SIGPIPE, SIG_IGN);
		}
};

IgnoreSigPipe initObj;
