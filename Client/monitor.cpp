// =====================================================================================
//
//       Filename:  try_monitor.cpp
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年03月29日 18时58分21秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  wangweihao (wangweihao), 578867817@qq.com
//        Company:  none
//
// =====================================================================================

#include "monitor.h"

int main(int argc, char **argv)
{
    Monitor monitor;
    std::string file(argv[1]);
    monitor.read_file(file);
    monitor.prompt();
}
