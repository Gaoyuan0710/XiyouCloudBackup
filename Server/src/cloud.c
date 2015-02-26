// =====================================================================================
//
//       Filename:  cloud.c
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年01月15日 13时04分08秒
//       Revision:  none
//       Compiler:  gcc
//
//         Author:  Shreck
//        
//
// =====================================================================================

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <pthread.h>
#include "pthread_pool.h"

#define MAX_EVENT_NUMBER 1024
#define PORT             3333
#define DUFFER_SIZE      1024


typedef struct
{
   int lenght;
   int tag;
   char content;
}DATA;


//将文件描述符设置为非阻塞
int setnonblocking(int fd)
{
    int old_option = fcntl(fd,FGETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd,FSETFL,new_option);
    return old_option;
}


//往注册表里注册fd事件
void addfd(int epollfd,int fd, oneshot)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN EPOLLET;

    if(oneshot)
    {
        event.events |= EPOLLONESHOT;
    }

    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
    setnonblocking(fd);
}

void reset_oneshot(int epollfd,int fd)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
    epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&event);
}


//对接受到的数据包做处理
void handle(int connfd)
{
    char buf[1024];
    int ret;
    DATA msg;
    int x;
    ret = recv(connfd,buf,1024,0);

    if(ret < 0)
    {
        printf("读操作出错，服务器将关闭该链接!\n");
        close(connfd);
    }

    memcpy(&msg,buf,sizeof(msg));   //字符串转化为结构体
    
    x = msg.tag;
    switch(x)
    {
        case 1:
            {
                //处理登陆
            }
        case 2:
            {
                //处理找回密码
            }
        case 3:
            {
                //处理上传
            }
        case 4:
            {
                //处理下载
            }
        case 5:
        case 6:
    }



}

int main(int argc,char **argv)
{
    char *ip;
    int port;
    int ret;
    struct sockaddr_in address,client_address;
    int listenfd;
    int connfd;
    int epollfd;
    struct epoll_event events[MAX_EVENT_NUMBER];
    int i;
    socklen_t len;


    
    if(argc < 3)
    {
        printf("您输入的参数有误!\n");
        exit(0);
    }
    
    //创建线程池
     if(thread_pool_create(20) != 0)
     {
        printf("线程池创建失败!\n");
        exit(1);
     }


    ip = argv[1];
    port = atoi(argv[2]);

    bzero(&address,sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&address.sin_addr);
    address.sin_port = htons(port);

    listenfd = socket(PF_INET,SOCK_STREAM,0);
    assert(listenfd >=  0);

    bind(listenfd,(struct sockaddr *)&address,sizeof(address));
    assert(ret != -1);

    ret = listen(listenfd,5);
    assert(ret != -1);

    //事件表的创建
    epollfd = epoll_create(30);
    assert(epollfd != -1);

    addfd(epollfd,listenfd,0);

    while(1)
    {

        ret = epoll_wait(epollfd,events,MAX_EVENT_NUMBER,-1);
        if(ret < 0)
        {
            printf("epoll failure\n");
            break;
        }

        for(i=0;i<ret;i++)
        {
            sockfd = events[i].data.fd;

            if(sockfd == listenfd)
            {
                len = sizeof(client_address);
                connfd = accept(listenfd,(struct sockaddr *)&client_address,&len);
                addfd(epollfd,connfd,1);
            }

            else if(events[i].events & EPOLLIN)
            {
                if(thread_pool_create(20) != 0)
                {
                    printf("线程池创建失败!\n");
                    exit(1);
                }
                
                //从线程池里分出一个线程来处理handle函数
                pool_add_work(handle,(void*)sockfd);
            }

            else if(events[i].events & EPOLLRDHUP)
            {
                close(sockfd);
                printf("一个客户端离开!\n");
            }

            else if(events[i].events & EPOLLERR)
            {
                printf("从%d获得一个错误\n",sockfd);
            }

            else if(events[i].events &EPOLLOUT)
            {
                
            }
            
            else 
            
            {
                printf("未知错误\n");
            }


        }
    }

    
}
