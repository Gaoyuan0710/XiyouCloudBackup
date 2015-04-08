// =====================================================================================
//
//       Filename:  monitor.cpp
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年03月29日 15时49分08秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  wangweihao (wangweihao), 578867817@qq.com
//        Company:  none
//
// =====================================================================================

#include "monitor.h"

const int event_size = sizeof(struct inotify_event);
const int buffer_size = 1024;

Monitor::Monitor()
{
    fd = inotify_init();
    assert(fd != -1);
}

int 
Monitor::read_file(std::string &file)
{
    std::ifstream in(file);
    std::string one_file;
    while(std::getline(in, one_file))
    {
        notify_add(one_file);
        one_file.clear();
    }
    return 0;
}

int 
Monitor::notify_add(std::string &one_list)
{
    int wd = inotify_add_watch(fd, one_list.c_str(), IN_MODIFY | IN_DELETE_SELF);
    //int ad = inotify_add_watch(fd, one_list.c_str(), IN_ALL_EVENTS);
    assert(wd != -1);
    file_list.push_back(one_list);
    watch.insert({one_list, wd});
    return 0;
}

int 
Monitor::notify_rm(std::string &one_list)
{
    auto iter = file_list.begin();
    for(; iter != file_list.end(); ++iter)
    {
        if(*iter == one_list)
        {
            break;
        }
    }
    if(iter == file_list.end())
        return 0;
    file_list.erase(iter);
    int ret = inotify_rm_watch(fd, watch[one_list]);
    assert(ret == 0);
    watch.erase(one_list);
    return 0;
}

void 
Monitor::prompt(void)
{
    int length = 0, i = 0;
    char buffer[buffer_size];
    memset(buffer, 0, buffer_size);
    /* 处理发生的事件，可能发生了多个事件 */
    while(length = read(fd, buffer, buffer_size))
    {
        i = 0;
        assert(length >= 0);
        /* 依次处理每个事件 */
        while(i < length)
        {
            struct inotify_event *event = (struct inotify_event*)(buffer + i);
            std::cout << "event_wd:" << event->wd << std::endl;
            std::cout << "event_mask:" << event->mask << std::endl;
            std::cout << "event_cookie:" << event->cookie << std::endl;
            std::cout << "event_len:" << event->len << std::endl;
            std::cout << "event_name:" << event->name << std::endl;
            if(event->len)
            {
                if(event->mask & IN_MODIFY)
                {
                    if(event->mask & IN_ISDIR)
                    {
                        std::cout << "the Directory:" << event->name << " is modify" << std::endl;
                    }
                    else
                    {
                        std::cout << "the file:" << event->name << " is modify" << std::endl;
                    }

                }
                else if(event->mask & IN_DELETE)
                {
                    if(event->mask & IN_ISDIR)
                    {
                        std::cout << "the Directory:" << event->name << " is delete" << std::endl;
                    }
                    else
                    {
                        std::cout << "the file:" << event->name << " is delete" << std::endl;
                    }       
                    notify_rm(fd, wd);
                }
            }
            i += event_size + event->len;
        }
    }
}
