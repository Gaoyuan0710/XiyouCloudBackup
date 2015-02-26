// =====================================================================================
// 
//       Filename:  CurrentThread.h
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年02月11日 17时27分49秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Gaoyuan, sishuiliunian0710@gmail.com
//        Company:  Class 1204 of Computer Science and Technology
// 
// =====================================================================================

#ifndef CURRENTTHREAD_H
#define CURRENTTHREAD_H

#include <unistd.h>
#include <sys/syscall.h>

namespace CurrentThread{
	extern __thread int t_cachedTid;
	inline void cacheTid(){
		t_cachedTid = static_cast <pid_t>
			(::syscall(SYS_gettid));
	}
	inline int tid(){
		if (t_cachedTid == 0){
			cacheTid();
		}
		return t_cachedTid;
	}
}

#endif
