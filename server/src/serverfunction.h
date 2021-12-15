#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <pthread.h>

#ifndef SERVERFUNCTION_H
#define SERVERFUNCTION_H

enum serverStatus{
    start,
    waitPass,
    loggedIn,
    reName
};

//描述一个连接：套接字及其关联的机器端口
struct connection{
    int conn_fd;
    bool valid;
    struct sockaddr_in addr;
};

struct transferInfo{
    int conn_fd;//for data
    int cmd_fd;//for command
    int startPlace;//for rest
    char name[256];
};

//描述服务器信息
struct FTPserver{
    enum serverStatus status;
    int mode;// 0 for default, 1 for PASV, 2 for PORT
    int sock_fd;
    int pasv_fd;
    char server_ip[18];
    char port_ip[18];
    char root_path[128];
    char OName[128];
    char work_directory[1024];
    int port_port;
    int restPlace;
    struct connection cmdConnection, dataConnection;
};

void fileTransfer(void* transfer_info);

void fileGet(void* transfer_info);

void CheckWrite(int conn_fd, char* buff, size_t len);

#endif