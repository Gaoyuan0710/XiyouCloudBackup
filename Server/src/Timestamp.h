// =====================================================================================
// 
//       Filename:  Timestamp.h
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年02月12日 16时22分17秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Gaoyuan, sishuiliunian0710@gmail.com
//        Company:  Class 1204 of Computer Science and Technology
// 
// =====================================================================================

#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <iostream>
#include <string>

using std::string;

namespace liunian{
class Timestamp{
	public:
		Timestamp():
			mircoSecondsSinceEpoch(0){}
		explicit Timestamp(int64_t miroSeconds);
		~Timestamp();
		bool valid();
		int64_t getMircoSecondsSinceEpoch();
		string toString() const;

		static Timestamp now();
		static Timestamp nowAfter(double seconds);
		static double nowMicroSeconds();
		static const int kMincroSecondsPerSecond = 1000 * 1000;

	private:
		int64_t mircoSecondsSinceEpoch;
};

inline bool operator < (Timestamp lhs, Timestamp rhs){
	return lhs.getMircoSecondsSinceEpoch() 
		< rhs.getMircoSecondsSinceEpoch();
}
inline bool operator == (Timestamp lhs, Timestamp rhs){
	return lhs.getMircoSecondsSinceEpoch() 
		== rhs.getMircoSecondsSinceEpoch();
}
inline Timestamp addTime(Timestamp timestamp, double seconds){
	int64_t delta = static_cast<int64_t> (
				seconds * Timestamp::kMincroSecondsPerSecond);
	return Timestamp(
				timestamp.getMircoSecondsSinceEpoch() + delta);


}
}
#endif
