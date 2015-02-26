// =====================================================================================
//
//       Filename:  errno.cpp
//
//    Description:
//
//        Version:  1.0
//        Created:  2014年10月15日 19时53分31秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Gaoyuan, sishuiliunian0710@gmail.com
//        Company:  Class 1204 of Computer Science and Technology
//
// =====================================================================================

#include <iostream>
#include <string>
#include <fstream>

using namespace std;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::fstream;

namespace liunian{
bool mylog(string filename, string err){
	ofstream outfile;

	outfile.open(filename.c_str(), ios::app | ios::ate);
	outfile.write(err.c_str(), (streamsize)err.length());

	outfile.close();
	return true;
}
}
