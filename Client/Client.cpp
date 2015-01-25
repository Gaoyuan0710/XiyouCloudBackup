// =====================================================================================
//
//       Filename:  Client.cpp
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年01月24日 08时38分45秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  wangweihao (wangweihao), 578867817@qq.com
//        Company:  none
//
// =====================================================================================

#include "Client.h"

FTP_admin::FTP_admin(void)
{

}

FTP_admin::~FTP_admin(void)
{

}

/* 登录，需要形如192.168.0.1:2234 */
int FTP_admin::Login_server(const std::string &ServerIp)
{
    std::string tmpip;
    std::string tmpport;
    std::size_t pos = ServerIp.find_first_of(":");
    
    /* 提取出ip 和 port */
    if(pos > 0)
    {
        tmpip = ServerIp.substr(0, pos-1);
        tmpport = ServerIp.substr(pos, ServerIp.size());
    }
    else
    {
        tmpip = ServerIp;
        tmpport = FTP_DEFAULT_PORT;
    }
    Server_ip = tmpip;
    Server_port = atoi(tmpport.c_str());
    
    /* 连接服务器 */
    if(Connect(Server_ip, Server_port) < 1)
    {
        printf("error:connect fault!\n");
        return -1;
    }

    return 0;
}

/* 连接服务器 */
int FTP_admin::Connect(const std::string &serverIp, std::size_t port)
{
    //连接前先选择好命令
    passWd = Select();

    /* 创建套接字
     * IPV4, TCP协议*/
    Socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(Socket_fd == ERROR_SOCKET)
    {
        printf("error:create socket fault!\n");
    }

    struct sockaddr_in address;

    //设置非阻塞
    //int flags = fcntl(sockfd, F_GETFL, 0);
    //fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(Server_port);
    int ret = inet_pton(AF_INET, Server_ip.c_str(), &address.sin_addr);
    if(ret == 0)
    {
        printf("error:inet_pton fault\n");
        return -1;
    }
    //异步IO不阻塞，返回值为-1
    //if(connect(sockfd, (struct sockaddr*)(&address), sizeof(address)) == -1)
    if(connect(Socket_fd, (struct sockaddr*)(&address), sizeof(address)) < 0)
    {
        printf("connect fault! \n");
        close(Socket_fd);
        return -1;
    }
    else
    {
        //连接选择并发送命令
        Send(Socket_fd, passWd);
    }
    
    
}

int FTP_admin::Send(int fd, const std::string &passWd)
{
    send(fd, passWd.c_str(), passWd.size(), 0);
    //根据命令来执行功能
    Exe_func(passWd);
}

std::string FTP_admin::Select(void)
{
    /* 根据点击的按钮来选择命令 */
    /*  */
}

const std::string FTP_admin::Explain_Order(void)
{
    std::map<std::string, std::string>Order;
    Order.insert(std::make_pair<std::string, std::string>("注册帐号", "31"));
    Order.insert(std::make_pair<std::string, std::string>("登录", "32"));
    Order.insert(std::make_pair<std::string, std::string>("找回密码", "33"));
    Order.insert(std::make_pair<std::string, std::string>("下载", "34"));
    Order.insert(std::make_pair<std::string, std::string>("上传", "35"));
    return Order.find(passWd)->second;

}

/* 本山写 */
void FTP_admin::Exe_func(const std::string &order)
{
    /* 参数passWd
     * 每个功能用单独的线程来执行
     * 执行完后关闭线程*/

    int passwd = atoi(order.c_str());
    switch(passwd)
    {
        case 31:/* 注册帐号 */
            
            break;
        case 32:/* 登录 */
            
            break;
        case 33:/* 找回密码 */

            break;
        case 34:/* 上传 */
            
            break;
        case 35:/* 下载 */
            
            break;
        default:
            break;
    }

}

/* 注册帐号 */
int FTP_admin::InputUsername(const std::string &username)
{

}

/* 登录 */
int FTP_admin::Register(void)
{

}

/* 找回密码 */
int FTP_admin::Find_password(void)
{

}

/* 上传 */
int FTP_admin::upload(const std::string &SerFile, const std::string &LocFile)
{

}

/* 下载 */
int FTP_admin::download(const std::string &SerFile, const std::string &LocFile)
{

}
