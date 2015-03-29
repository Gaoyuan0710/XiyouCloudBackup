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

Monitor::Monitor()
{
    fd = inotify_init();
    assert(fd != -1);
    event_ret.push_back("file was accessed");
    event_ret.push_back("file was modified");
    event_ret.push_back("file attributes were changed");
    event_ret.push_back("writtable file closed");
    event_ret.push_back("unwrittable file closed");
    event_ret.push_back("file was opened");
    event_ret.push_back("file was move from X");
    event_ret.push_back("file was moved to Y");
    event_ret.push_back("subfile was created");
    event_ret.push_back("subfile was deleted");
    event_ret.push_back("self was deleted");
    event_ret.push_back("self was moved");
    event_ret.push_back("");
    event_ret.push_back("backing fd was unmounted");
    event_ret.push_back("event queued overflowed");
    event_ret.push_back("file was ignored");

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
    int ad = inotify_add_watch(fd, one_list.c_str(), IN_MODIFY | IN_DELETE | IN_MOVE | IN_OPEN);
    assert(ad != -1);
    file_list.push_back(one_list);
    watch.insert({one_list, ad});
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
    int len;
    struct inotify_event * event;
    char buffer[BUFFER_SIZE];
    char strbuf[16];
    char *offset = NULL;
    while(len = read(fd, buffer, BUFFER_SIZE))
    {
        offset = buffer;
        std::cout << "len:" << len << std::endl;
        event = (struct inotify_event*)buffer;
        while(((char*)event - buffer) < len)
        {
            if(event->mask & IN_ISDIR)
            {
                memcpy(strbuf, "Directory", 11);
            }
            else
            {
                memcpy(strbuf, "file", 5);
            }
            printf("Object type:%s\n", strbuf);
            for(auto iter = watch.begin(); iter != watch.end(); ++iter)
            {
                if(event->wd == iter->second)
                {
                    std::cout << "Object name:" << iter->first << std::endl;
                    break;
                }
            }
            printf("Event mask:%08X\n",event->mask);
            for(int i = 0; i < event_ret.size(); ++i)
            {
                if(event->mask & (1 << i))
                {
                    std::cout << "event:" << event_ret[i] << std::endl;
                }
            }
            int tmp_len = sizeof(struct inotify_event) + event->len;
            event = (struct inotify_event*)(offset + tmp_len);
            offset += tmp_len;
        }
    }
}
