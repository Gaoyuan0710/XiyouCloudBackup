// =====================================================================================
// 
//       Filename:  Timestamp.cpp
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年02月12日 17时48分36秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Gaoyuan, sishuiliunian0710@gmail.com
//        Company:  Class 1204 of Computer Science and Technology
// 
// =====================================================================================
#include <sys/time.h>
#include <stdio.h>

#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#undef __STDC_FORMAT_MACROS


#include "Timestamp.h"


using namespace liunian;



Timestamp::Timestamp(int64_t mircoSeconds)
		:mircoSecondsSinceEpoch(mircoSeconds)
{
}
Timestamp::~Timestamp(){
}
bool Timestamp::valid(){
	return mircoSecondsSinceEpoch > 0;
}
int64_t Timestamp::getMircoSecondsSinceEpoch(){
	return mircoSecondsSinceEpoch;
} 
string Timestamp::toString() const{
	char buf[32] = {0};
	int64_t seconds = 
		mircoSecondsSinceEpoch / kMincroSecondsPerSecond;
	int64_t mircoSeconds = 
		mircoSecondsSinceEpoch % kMincroSecondsPerSecond;
	snprintf(buf, sizeof(buf) - 1,
				"%" PRId64 "%06" PRId64 "",
				seconds, mircoSeconds);
	return buf;
}
Timestamp Timestamp::now(){
	return Timestamp(Timestamp::nowMicroSeconds());
}
Timestamp Timestamp::nowAfter(double seconds){
	return Timestamp(Timestamp::nowMicroSeconds() 
				+ kMincroSecondsPerSecond * seconds);
}
double Timestamp::nowMicroSeconds(){
	struct timeval tv;

	gettimeofday(&tv, NULL);
	int64_t seconds = tv.tv_sec;
	return seconds * kMincroSecondsPerSecond 
		+ tv.tv_usec;
}



