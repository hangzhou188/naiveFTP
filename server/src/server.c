#include "handle.h"
#include "serverfunction.h"

struct FTPserver ftpServer;

int serverInit(struct sockaddr_in *server_addr, int port, char* server_root);

int main(int argc, char *argv[])
{
    int server_port = 21;
    char server_root[128];
    strcpy(server_root,"/tmp");
    if (argc >= 2 && argc < 4)
    {
        if (!strcmp(argv[1], "-port"))
        {
            if (argc < 2)
            {
                //printf("Please specify the port number.\r\n");
                exit(1);
            }
            else
            {
                int port_num = atoi(argv[2]);
                server_port = port_num;
            }
        }
    }
    else if (argc >= 4)
    {
        if (!strcmp(argv[1], "-port"))
        {
            if (argc < 2)
            {
                //printf("Please specify the port number.\r\n");
                exit(1);
            }
            else
            {
                int port_num = atoi(argv[2]);
                server_port = port_num;
            }
        }
        if(!strcmp(argv[3], "-root")) {
            bzero(server_root,sizeof(server_root));
            strcpy(server_root, argv[4]);
        }
    }
    int sock_fd, conn_fd;
    struct sockaddr_in *server_addr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
    char buff[BUF_SIZE];
    int ret;
    sock_fd = serverInit(server_addr, server_port, server_root);
    while (1)
    {
        // 接受请求
        if ((conn_fd = accept(sock_fd, (struct sockaddr *)NULL, NULL)) == -1)
        {
            //printf("accept socket error: %s\n\a", strerror(errno));
            continue;
        }
        //printf("client accepted.\r\n");
        pid_t Processid = fork();
        if (Processid == 0) {
            struct connection cmdConnection;
            cmdConnection.conn_fd = conn_fd;
            cmdConnection.addr = *server_addr;
            ftpServer.cmdConnection = cmdConnection;
            CheckWrite(ftpServer.cmdConnection.conn_fd, "220 Welcome.\r\n", 14);
            while (1)
            {
                // 接受数据
                bzero(buff, sizeof(buff));
                ret = recv(conn_fd, buff, BUF_SIZE, 0);
                struct FTPrequest request = parseBuff(buff);
                if (ret <= 0)
                {
                    // 客户端关闭
                    //printf("client close\n");
                    close(conn_fd);
                    break;
                }
                else
                {
                    // 添加结束符
                    if (ret < BUF_SIZE)
                    {
                        memset(&buff[ret], '\0', 1);
                    }
                    //printf("recv msg from client: %s\n", buff);
                    //printf("parameter: %s\r\n", request.operand);
                    requestHandle(request, conn_fd, &ftpServer);
                }
            }
            //free(server_addr);
            //close(conn_fd);
            close(sock_fd);
            exit(0);
        } else if (Processid > 0) {
            close(conn_fd);
        }
    }
    free(server_addr);
    close(sock_fd);
    return 0;
}

int serverInit(struct sockaddr_in *server_addr, int port, char* server_root)
{   
    int port_number = port;

    //printf("port: %d\r\n", port_number);
    int sock_fd;
    // 创建socket描述符
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "Socket error:%s\n\a", strerror(errno));
        exit(1);
    }
    int one = 1;
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));//解决评测脚本短时间内连续使用无法绑定端口的问题
    ftpServer.sock_fd = sock_fd;
    strcpy(ftpServer.server_ip,"0.0.0.0");
    // 填充sockaddr_in结构
    bzero(server_addr, sizeof(struct sockaddr_in));
    server_addr->sin_family = AF_INET;
    server_addr->sin_addr.s_addr = inet_addr(ftpServer.server_ip);
    server_addr->sin_port = htons(port_number);
    // 绑定sock_fd描述符:将套接字sock_fd与端口server_addr关联起来
    if (bind(sock_fd, (struct sockaddr *)(server_addr), sizeof(struct sockaddr)) == -1)
    {
        fprintf(stderr, "Bind error:%s\n\a", strerror(errno));
        exit(1);
    }
    // 监听sock_fd描述符
    if (listen(sock_fd, 20) == -1)
    {
        fprintf(stderr, "Listen error:%s\n\a", strerror(errno));
        exit(1);
    }
    ftpServer.status = start;
    ftpServer.cmdConnection.valid = false;
    ftpServer.dataConnection.valid = false;
    strcpy(ftpServer.root_path, server_root);
    strcpy(ftpServer.work_directory, "/");
    chdir(ftpServer.root_path);
    char root[128];
    getcwd(root,sizeof(root));
    strcpy(ftpServer.root_path,root);
    if (access(server_root, F_OK) != 0) {
        mkdir(server_root, S_IRWXO);
    }
    return sock_fd;
}