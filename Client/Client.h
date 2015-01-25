// =====================================================================================
//
//       Filename:  Client.h
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年01月18日 09时49分02秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  wangweihao (wangweihao), 578867817@qq.com
//        Company:  none
//
// =====================================================================================

#ifndef _CLIENT_H_
#define _CLIENT_H_

/* c++ */
#include <iostream>
#include <string>
#include <fcntl.h>       //fcntl
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>   //inet_pton
#include <assert.h>
#include <sys/types.h>   //connect
#include <sys/socket.h>
#include <sys/select.h>
#include <pthread.h>     //pthread_create
#include <map>           //map


//命令
#define ERROR_SOCKET -1
#define FTP_DEFAULT_PORT "21"
#define FTP_DEFAULT_BUFFER 4096
/* 命令 */
enum FTP_MESG
{
    FTP_ORDER_USERNAME = 31,
    FTP_ORDER_PASSWORD, 
    FTP_ORDER_QUIT,
    FPT_ORDER_DOWNLOAD,
    FPT_ORDER_UPLOAD
};


/* 连接类 */
class FTP_admin
{
    public:
        FTP_admin(void);
        ~FTP_admin(void);
        //登录服务器
        int Login_server(const std::string &ServerIp);
        //退出服务器
        int Quit_server(void);
        //解析命令
        const std::string Explain_Order(void);
        //上传
        int upload(const std::string &SerFile, const std::string &LocFile);
        //下载
        int download(const std::string &SerFile, const std::string &LocFile);
        //根据命令执行功能
        void Exe_func(const std::string &order);

        //Input username
        //单独出来注册时要单发送username检验是否合法
        int InputUsername(const std::string &username);
        //Input userpassword
        int InputUserpassword(const std::string &userpassword);
        //登录
        int Register(void);
        //找回密码
        int Find_password(void);

    private:
        //连接服务器
        int Connect(const std::string &serverIp, std::size_t port);
        //发送命令
        int Send(int fd, const std::string &passWd);
        //选择命令
        std::string Select(void);

    private:
        //命令
        std::string passWd;
        //帐号密码
        std::string username;
        std::string userpassword;
        //服务器Ip和Port
        std::string Server_ip;
        std::size_t Server_port;
        int Socket_fd;

};


#endif
