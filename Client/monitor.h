// =====================================================================================
//
//       Filename:  monitor.cpp
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年03月29日 15时27分56秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  wangweihao (wangweihao), 578867817@qq.com
//        Company:  none
//
// =====================================================================================

#ifndef _MONITOR_H_
#define _MONITOR_H_

#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sys/inotify.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 1024

class Monitor
{
    public:
        Monitor();
        /* 读取配置文件需检测的目录 */
        int read_file(std::string &file);
        /* 加入事件到监测的目录 */
        int notify_add(std::string &one_list);
        /* 删除事件从监测的目录 */
        int notify_rm(std::string &one_list);
        /* 显示监测中被改变的 */
        void prompt(void);
            

    private:
        /* 存储监控的文件列表 */
        std::vector<std::string> file_list;
        /* 文件列表对应的wd值 */
        std::map<std::string, int> watch;
        /* 文件描述符 */
        int fd;
        /* 对应返回信息 */
        std::vector<std::string> event_ret;
};


#endif
