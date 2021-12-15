#include "handle.h"

char *trimSpace(char *buff)
{
    if (buff == NULL || *buff == '\0')
    {
        return buff;
    }
    char *p = buff;
    while (*p != '\0' && isspace(*p))
    {
        p++;
        buff++;
    }
    int bufflen = strlen(p);
    if (bufflen <= 0)
    {
        return p;
    }
    for (int i = bufflen - 1; i >= 0; i--)
    {
        if (isspace(buff[i]))
        {
            buff[i] = '\0';
        }
        else
            break;
    }
    return buff;
}

/*
去除一个路径最后一个'/'及之后的内容
路径为根目录'/'时不去除
路径为"/dir"形式时将其变为"/"
*/
void trimBackSlash(char *buff)
{
    int len = strlen(buff);
    if (len < 2)
    {
        return;
    }
    int trimStart = len - 2, trimEnd = len - 1;
    for (; trimStart > 0; trimStart--)
    {
        if (buff[trimStart] == '/')
        {
            break;
        }
    }
    if (trimStart == 0)
    {
        trimStart = trimStart + 1;
    }
    for (int p = trimStart; p <= trimEnd; p++)
    {
        buff[p] = '\0';
    }
}

bool splitBefore(char *dest, char *resc, int place)
{
    if (place < 1 || place > 6)
    {
        return false;
    }
    int len = strlen(resc);
    int splits[7];
    splits[0] = -1;
    splits[6] = len - 1;
    int count = 0;
    for (int i = 0; i < len; i++)
    {
        if (resc[i] == ',')
        {
            ++count;
            splits[count] = i;
            resc[i] = '.';
        }
    }
    if (count != 5)
    {
        return false;
    }
    int start = 0, end = splits[place];
    for (int i = start; i < end; i++)
    {
        dest[i - start] = resc[i];
    }
    dest[end - start] = '\0';
    return true;
}

bool seekSplit(char *dest, char *resc, int place)
{
    if (place < 1 || place > 6)
    {
        return false;
    }
    int len = strlen(resc);
    int splits[7];
    splits[0] = -1;
    splits[6] = len;
    int count = 0;
    for (int i = 0; i < len; i++)
    {
        if (resc[i] == ',' || resc[i] == '.')
        {
            ++count;
            splits[count] = i;
        }
    }
    if (count != 5)
    {
        return false;
    }
    int start = splits[place - 1] + 1, end = splits[place];
    for (int i = start; i < end; i++)
    {
        dest[i - start] = resc[i];
    }
    dest[end - start] = '\0';
    return true;
}

void transferIP(char* dest, char* src){
    strcpy(dest, src);
    int len = strlen(dest);
    for(int i = 0; i<len; i++) {
        if(dest[i]=='.') {
            dest[i]=',';
        } 
    }
}

bool isPositiveNumber(char* m_str) {
    int len = strlen(m_str);
    for (int i = 0; i<len; i++) {
        if(!isdigit(m_str[i])) {
            return false;
        }
    }
    if(m_str[0] == '0') {
        return false;
    }
    return true;
}

struct FTPrequest parseBuff(char *buff)
{
    char *buffTrim = (char *)malloc(sizeof(char) * BUF_SIZE);
    bzero(buffTrim, 512);
    strcpy(buffTrim, buff);
    buffTrim = trimSpace(buffTrim);
    struct FTPrequest request;
    request.type = UNDEFINED;
    bzero(request.operand, sizeof(request.operand));
    if (strlen(buffTrim) <= 2)
    {
        return request;
    }
    if (strlen(buffTrim) >= 3)
    {
        bool succ = false;
        if (buffTrim[0] == 'C' && buffTrim[1] == 'W' && buffTrim[2] == 'D')
        {
            request.type = CWD;
            char *p = buffTrim + 3;
            p = trimSpace(p);
            strcpy(request.operand, p);
            succ = true;
        }
        else if (strlen(buffTrim) == 3 && buffTrim[0] == 'P' && buffTrim[1] == 'W' && buffTrim[2] == 'D')
        {
            request.type = PWD;
            succ = true;
        }
        else if (strlen(buffTrim) >= 3 && buffTrim[0] == 'M' && buffTrim[1] == 'K' && buffTrim[2] == 'D')
        {
            request.type = MKD;
            char *p = buffTrim + 3;
            p = trimSpace(p);
            strcpy(request.operand, p);
            succ = true;
        }
        else if (strlen(buffTrim) >= 3 && buffTrim[0] == 'R' && buffTrim[1] == 'M' && buffTrim[2] == 'D')
        {
            request.type = RMD;
            char *p = buffTrim + 3;
            p = trimSpace(p);
            strcpy(request.operand, p);
            succ = true;
        }
        if (succ)
        {
            return request;
        }
    }
    if (strlen(buffTrim) >= 4)
    {
        if (buffTrim[0] == 'U' && buffTrim[1] == 'S' && buffTrim[2] == 'E' && buffTrim[3] == 'R')
        {
            request.type = USER;
            char *p = buffTrim + 4;
            p = trimSpace(p);
            strcpy(request.operand, p);
        }
        else if (buffTrim[0] == 'P' && buffTrim[1] == 'A' && buffTrim[2] == 'S' && buffTrim[3] == 'S')
        {
            request.type = PASS;
            char *p = buffTrim + 4;
            p = trimSpace(p);
            strcpy(request.operand, p);
        }
        else if (strlen(buffTrim) == 4 && buffTrim[0] == 'P' && buffTrim[1] == 'A' && buffTrim[2] == 'S' && buffTrim[3] == 'V')
        {
            request.type = PASV;
        }
        else if (strlen(buffTrim) == 4 && buffTrim[0] == 'Q' && buffTrim[1] == 'U' && buffTrim[2] == 'I' && buffTrim[3] == 'T')
        {
            request.type = QUIT;
        }
        else if (strlen(buffTrim) == 4 && buffTrim[0] == 'L' && buffTrim[1] == 'I' && buffTrim[2] == 'S' && buffTrim[3] == 'T')
        {
            request.type = LIST;
        }
        else if (buffTrim[0] == 'P' && buffTrim[1] == 'O' && buffTrim[2] == 'R' && buffTrim[3] == 'T')
        {
            request.type = PORT;
            char *p = buffTrim + 4;
            p = trimSpace(p);
            strcpy(request.operand, p);
        }
        else if (buffTrim[0] == 'R' && buffTrim[1] == 'E' && buffTrim[2] == 'T' && buffTrim[3] == 'R')
        {
            request.type = RETR;
            char *p = buffTrim + 4;
            p = trimSpace(p);
            strcpy(request.operand, p);
        }
        else if (buffTrim[0] == 'R' && buffTrim[1] == 'N' && buffTrim[2] == 'F' && buffTrim[3] == 'R')
        {
            request.type = RNFR;
            char *p = buffTrim + 4;
            p = trimSpace(p);
            strcpy(request.operand, p);
        }
        else if (buffTrim[0] == 'R' && buffTrim[1] == 'N' && buffTrim[2] == 'T' && buffTrim[3] == 'O')
        {
            request.type = RNTO;
            char *p = buffTrim + 4;
            p = trimSpace(p);
            strcpy(request.operand, p);
        }
        else if (buffTrim[0] == 'S' && buffTrim[1] == 'T' && buffTrim[2] == 'O' && buffTrim[3] == 'R')
        {
            request.type = STOR;
            char *p = buffTrim + 4;
            p = trimSpace(p);
            strcpy(request.operand, p);
        }
        else if (buffTrim[0] == 'A' && buffTrim[1] == 'P' && buffTrim[2] == 'P' && buffTrim[3] == 'E')
        {
            request.type = APPE;
            char *p = buffTrim + 4;
            p = trimSpace(p);
            strcpy(request.operand, p);
        }
        else if (strlen(buffTrim) == 4 && buffTrim[0] == 'S' && buffTrim[1] == 'Y' && buffTrim[2] == 'S' && buffTrim[3] == 'T')
        {
            request.type = SYST;
        }
        else if (buffTrim[0] == 'T' && buffTrim[1] == 'Y' && buffTrim[2] == 'P' && buffTrim[3] == 'E')
        {
            request.type = TYPE;
            char *p = buffTrim + 4;
            p = trimSpace(p);
            strcpy(request.operand, p);
        }
        else if (buffTrim[0] == 'A' && buffTrim[1] == 'B' && buffTrim[2] == 'O' && buffTrim[3] == 'R')
        {
            request.type = TYPE;
            char *p = buffTrim + 4;
            p = trimSpace(p);
            strcpy(request.operand, p);
        }
        else if (buffTrim[0] == 'R' && buffTrim[1] == 'E' && buffTrim[2] == 'S' && buffTrim[3] == 'T')
        {
            request.type = REST;
            char *p = buffTrim + 4;
            p = trimSpace(p);
            strcpy(request.operand, p);
        }
    }
    free(buffTrim);
    return request;
}

void handle_USER(struct FTPserver *ftpServer, char *userName, int conn_fd)
{
    if (!strcmp("anonymous", userName))
    {
        ftpServer->status = waitPass;
        CheckWrite(ftpServer->cmdConnection.conn_fd, "331 User name okay, need password.\r\n", 36);
    }
    else
    {
        CheckWrite(ftpServer->cmdConnection.conn_fd, "530 Not logged in.\r\n", 20);
    }
}

void handle_PASS(struct FTPserver *ftpServer, char *passWord)
{
    if (ftpServer->status == waitPass)
    {
        CheckWrite(ftpServer->cmdConnection.conn_fd, "230 User logged in, proceed.\r\n", 30);
        ftpServer->status = loggedIn;
    }
    else
    {
        CheckWrite(ftpServer->cmdConnection.conn_fd, "503 Bad sequence of commands.\r\n", 31);
        ftpServer->status = start;
    }
}

void handle_CWD(struct FTPserver *ftpServer, char *path)
{
    if (ftpServer->status == start || ftpServer->status == waitPass)
    {
        CheckWrite(ftpServer->cmdConnection.conn_fd, "530 Not logged in.\r\n", 20);
        ftpServer->status = start;
        return;
    }
    char target_path[1152];
    char cur_path[1152];
    sprintf(cur_path, "%s%s", ftpServer->root_path, ftpServer->work_directory);
    //非绝对路径
    if (path[0] != '/')
    {
        sprintf(target_path, "%s%s/%s", ftpServer->root_path, ftpServer->work_directory, path);
    }
    //绝对路径
    else
    {
        sprintf(target_path, "%s%s", ftpServer->root_path, path);
    }
    if (chdir(target_path) != 0)
    {
        char retBuff[128];
        sprintf(retBuff, "550 %s: No such directory.\r\n", path);
        CheckWrite(ftpServer->cmdConnection.conn_fd, retBuff, strlen(retBuff));
        return;
    }
    else
    {
        char newDir[1200];
        getcwd(newDir, sizeof(newDir));
        if (strlen(ftpServer->root_path) > strlen(newDir))
        {
            CheckWrite(ftpServer->cmdConnection.conn_fd, "550 Cannot change path.\r\n", 25);
            chdir(cur_path);
            return;
        }
        else
        {
            sprintf(ftpServer->work_directory, "%s", newDir + strlen(ftpServer->root_path));
            if(ftpServer->work_directory[0] == '\0') {
                ftpServer->work_directory[0] = '/';
                ftpServer->work_directory[1] = '\0';
            }
            CheckWrite(ftpServer->cmdConnection.conn_fd, "250 Command CWD OK.\r\n", 21);
        }
    }
}

void handle_PWD(struct FTPserver *ftpServer)
{
    if (ftpServer->status == start || ftpServer->status == waitPass)
    {
        CheckWrite(ftpServer->cmdConnection.conn_fd, "530 Not logged in.\r\n", 20);
        ftpServer->status = start;
        return;
    }
    char retBuff[1040];
    sprintf(retBuff, "257 \"%s\".\r\n", ftpServer->work_directory);
    CheckWrite(ftpServer->cmdConnection.conn_fd, retBuff, strlen(retBuff));
}

void handle_MKD(struct FTPserver *ftpServer, char *DName)
{
    if (ftpServer->status == start || ftpServer->status == waitPass)
    {
        CheckWrite(ftpServer->cmdConnection.conn_fd, "530 Not logged in.\r\n", 20);
        ftpServer->status = start;
        return;
    }
    char targetFilePath[1200];
    sprintf(targetFilePath, "%s%s/%s", ftpServer->root_path, ftpServer->work_directory, DName);
    if (access(targetFilePath, F_OK) == 0)
    {
        CheckWrite(ftpServer->cmdConnection.conn_fd, "550 Already exists.\r\n", 21);
        return;
    }
    else
    {
        if (mkdir(targetFilePath, 0777) == 0)
        {
            chmod(targetFilePath, 01777);
            CheckWrite(ftpServer->cmdConnection.conn_fd, "250 Directory created.\r\n", 24);
            return;
        }
        else
        {
            CheckWrite(ftpServer->cmdConnection.conn_fd, "550 Cannot create directory.\r\n", 24);
            return;
        }
    }
}

void handle_RMD(struct FTPserver *ftpServer, char *DName)
{
    if (ftpServer->status == start || ftpServer->status == waitPass)
    {
        CheckWrite(ftpServer->cmdConnection.conn_fd, "530 Not logged in.\r\n", 20);
        ftpServer->status = start;
        return;
    }
    char command[128];
    sprintf(command,"rm -r %s", DName);
    FILE *fp;
    if ((fp = popen(command, "r")) == NULL) {
        CheckWrite(ftpServer->cmdConnection.conn_fd, "550 Cannot remove file.\r\n", 25);
    } else {
        CheckWrite(ftpServer->cmdConnection.conn_fd, "250 Command RMD OK.\r\n", 21);
    }
    pclose(fp);
}

void handle_PASV(struct FTPserver *ftpServer)
{
    if (ftpServer->status == start || ftpServer->status == waitPass)
    {
        CheckWrite(ftpServer->cmdConnection.conn_fd, "530 Not logged in.\r\n", 20);
        ftpServer->status = start;
        return;
    }
    if (ftpServer->dataConnection.valid)
    {
        close(ftpServer->dataConnection.conn_fd);
    }
    int randPort = generateRandPort();
    int data_fd;
    // 创建socket描述符
    if ((data_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "Socket error:%s\n\a", strerror(errno));
        exit(1);
    }
    // 填充sockaddr_in结构
    struct sockaddr_in *data_addr = &ftpServer->dataConnection.addr;
    bzero(data_addr, sizeof(struct sockaddr_in));
    data_addr->sin_family = AF_INET;
    data_addr->sin_addr.s_addr = inet_addr(ftpServer->server_ip);
    data_addr->sin_port = htons(randPort);
    // 绑定sock_fd描述符:将套接字sock_fd与端口server_addr关联起来
    if (bind(data_fd, (struct sockaddr *)(data_addr), sizeof(struct sockaddr)) == -1)
    {
        fprintf(stderr, "Bind error:%s\n\a", strerror(errno));
        exit(1);
    }
    // 监听sock_fd描述符
    if (listen(data_fd, 20) == -1)
    {
        fprintf(stderr, "Listen error:%s\n\a", strerror(errno));
        exit(1);
    }
    char responseBuff[60];
    char ip[18];
    transferIP(ip, ftpServer->server_ip);
    //printf("randPort:%d\r\n", randPort);
    sprintf(responseBuff, "227 Entering passive mode (%s,%d,%d)\r\n", ip, randPort / 256, randPort % 256);
    //printf("%s\r\n", responseBuff);
    ftpServer->dataConnection.conn_fd = data_fd;
    ftpServer->dataConnection.valid = true;
    CheckWrite(ftpServer->cmdConnection.conn_fd, responseBuff, strlen(responseBuff));
    ftpServer->mode = 1;
};

void handle_LIST(struct FTPserver *ftpServer)
{
    if (ftpServer->status == start || ftpServer->status == waitPass)
    {
        CheckWrite(ftpServer->cmdConnection.conn_fd, "530 Not logged in.\r\n", 20);
        ftpServer->status = start;
        return;
    }
    char command[12];
    //sprintf(command, "cd %s%s;ls -n", ftpServer->root_path, ftpServer->work_directory);
    strcpy(command,"ls -n");
    char buff[1024];
    FILE *fp;
    if ((fp = popen(command, "r")) == NULL)
    {
        //printf("File open error: %s\r\n", strerror(errno));
    } else {
        fgets(buff,1024,fp);
    }
    if (ftpServer->mode == 0)
    {
        CheckWrite(ftpServer->cmdConnection.conn_fd, "PASV or PORT mode needed.\r\n", 27);
        return;
    }
    else if (ftpServer->mode == 1)
    {
        bzero(buff,sizeof(buff));
        int len = 0;
        ftpServer->pasv_fd = accept(ftpServer->dataConnection.conn_fd, (struct sockaddr *)NULL, NULL);
        //printf("pasv client connected.\r\n");
        CheckWrite(ftpServer->cmdConnection.conn_fd, "150 Opening data connection.\r\n", 30);
        while ((len = fread(buff, sizeof(char), sizeof(buff), fp)) > 0)
        {
            CheckWrite(ftpServer->pasv_fd, buff, len);
            bzero(buff, sizeof(buff));
        }
        close(ftpServer->pasv_fd);
    }
    else if (ftpServer->mode == 2)
    {
        if ((ftpServer->dataConnection.conn_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            fprintf(stderr, "Socket error:%s\n\a", strerror(errno));
            exit(1);
        }
        else
        {
            CheckWrite(ftpServer->cmdConnection.conn_fd, "150 Opening data connection.\r\n", 30);
        }
        struct sockaddr_in *data_addr = &ftpServer->dataConnection.addr;
        bzero(data_addr, sizeof(struct sockaddr_in));
        data_addr->sin_family = AF_INET;
        data_addr->sin_addr.s_addr = inet_addr(ftpServer->port_ip);
        data_addr->sin_port = htons(ftpServer->port_port);
        if (connect(ftpServer->dataConnection.conn_fd, (struct sockaddr *)data_addr, sizeof(struct sockaddr)) == -1)
        {
            fprintf(stderr, "Connect error:%s\n\a", strerror(errno));
            exit(1);
        }
        int len;
        while ((len = fread(buff, sizeof(char), sizeof(buff), fp)) > 0)
        {
            CheckWrite(ftpServer->dataConnection.conn_fd, buff, len);
            bzero(buff, sizeof(buff));
        }
        close(ftpServer->dataConnection.conn_fd);
    }
    pclose(fp);
    CheckWrite(ftpServer->cmdConnection.conn_fd, "226 Command LIST okay.\r\n", 24);
    ftpServer->mode = 0;
}

void handle_PORT(struct FTPserver *ftpServer, char *ip_port)
{
    if (ftpServer->status == start || ftpServer->status == waitPass)
    {
        CheckWrite(ftpServer->cmdConnection.conn_fd, "530 Not logged in.\r\n", 20);
        ftpServer->status = start;
        return;
    }
    if (ftpServer->dataConnection.valid)
    {
        close(ftpServer->dataConnection.conn_fd);
    }
    if (splitBefore(ftpServer->port_ip, ip_port, 4) == true)
    {
        char p1[4], p2[4];
        if (seekSplit(p1, ip_port, 5) == true && seekSplit(p2, ip_port, 6) == true)
        {
            int n_h1 = atoi(p1), n_h2 = atoi(p2);
            ftpServer->port_port = n_h1 * 256 + n_h2;
            CheckWrite(ftpServer->cmdConnection.conn_fd, "200 Command PORT okay.\r\n", 24);
            //printf("PORT OK \r\n");
            ftpServer->mode = 2;
        }
        else
        {
            CheckWrite(ftpServer->cmdConnection.conn_fd, "510 Syntax error in parameters.\r\n", 34);
            ftpServer->mode = 0;
            return;
        }
    }
    else
    {
        CheckWrite(ftpServer->cmdConnection.conn_fd, "510 Syntax error in parameters.\r\n", 34);
        ftpServer->mode = 0;
        return;
    }
    //printf("addr: %s, port: %d\r\n", ftpServer->port_ip, ftpServer->port_port);
}

void handle_QUIT(struct FTPserver *ftpServer)
{
    CheckWrite(ftpServer->cmdConnection.conn_fd, "221 Goodbye.\r\n", 14);
    close(ftpServer->dataConnection.conn_fd);
    close(ftpServer->pasv_fd);
    close(ftpServer->cmdConnection.conn_fd);
    exit(0);
}

void handle_RETR(struct FTPserver *ftpServer, char *filePath)
{
    if (ftpServer->status == start || ftpServer->status == waitPass)
    {
        CheckWrite(ftpServer->cmdConnection.conn_fd, "530 Not logged in.\r\n", 20);
        ftpServer->status = start;
        return;
    }
    if (ftpServer->mode == 0)
    {
        CheckWrite(ftpServer->cmdConnection.conn_fd, "PASV or PORT mode needed.\r\n", 27);
        return;
    }
    if (strlen(filePath) == 0)
    {
        CheckWrite(ftpServer->cmdConnection.conn_fd, "501 Syntax error.\r\n", 19);
        return;
    }
    if (ftpServer->mode == 1)
    {
        int conn_fd;
        conn_fd = accept(ftpServer->dataConnection.conn_fd, (struct sockaddr *)NULL, NULL);
        CheckWrite(ftpServer->cmdConnection.conn_fd, "150 Opening data connection.\r\n", 30);
        ftpServer->pasv_fd = conn_fd;
        struct transferInfo transfer_info;
        transfer_info.conn_fd = ftpServer->pasv_fd;
        transfer_info.cmd_fd = ftpServer->cmdConnection.conn_fd;
        strcpy(transfer_info.name, filePath);
        transfer_info.startPlace = ftpServer->restPlace;
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, (void *)(&fileTransfer), (void *)(&transfer_info)) == -1) {
            //printf("pthread create error: %s\n\a", strerror(errno));
            return;
        }
    }
    else if (ftpServer->mode == 2)
    {
        if ((ftpServer->dataConnection.conn_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            fprintf(stderr, "Socket error:%s\n\a", strerror(errno));
            exit(1);
        }
        struct sockaddr_in *data_addr = &ftpServer->dataConnection.addr;
        bzero(data_addr, sizeof(struct sockaddr_in));
        data_addr->sin_family = AF_INET;
        data_addr->sin_addr.s_addr = inet_addr(ftpServer->port_ip);
        data_addr->sin_port = htons(ftpServer->port_port);
        if (connect(ftpServer->dataConnection.conn_fd, (struct sockaddr *)data_addr, sizeof(struct sockaddr)) == -1)
        {
            fprintf(stderr, "Connect error:%s\n\a", strerror(errno));
            CheckWrite(ftpServer->cmdConnection.conn_fd, "426 Data connection error.\r\n", 28);
        }
        else
        {
            CheckWrite(ftpServer->cmdConnection.conn_fd, "150 Opening data connection.\r\n", 30);
        }
        struct transferInfo transfer_info;
        transfer_info.conn_fd = ftpServer->dataConnection.conn_fd;
        transfer_info.cmd_fd = ftpServer->cmdConnection.conn_fd;
        strcpy(transfer_info.name, filePath);
        transfer_info.startPlace = ftpServer->restPlace;
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, (void *)(&fileTransfer), (void *)(&transfer_info)) == -1) {
            //printf("pthread create error: %s\n\a", strerror(errno));
            return;
        }
    }
    //printf("RETR OK\r\n");
    ftpServer->mode = 0;
    ftpServer->restPlace = 0;
}

void handle_REST(struct FTPserver* ftpServer, char* restPlaceSTR) {
    if (ftpServer->status == start || ftpServer->status == waitPass)
    {
        CheckWrite(ftpServer->cmdConnection.conn_fd, "530 Not logged in.\r\n", 20);
        ftpServer->status = start;
        return;
    }
    if(!isPositiveNumber(restPlaceSTR)) {
        CheckWrite(ftpServer->cmdConnection.conn_fd, "550 syntax error.\r\n", 19);
        return;
    }
    else {
        ftpServer->restPlace = atoi(restPlaceSTR);
        CheckWrite(ftpServer->cmdConnection.conn_fd, "350 Command REST OK.\r\n", 22);
    }
}

void handle_RNFR(struct FTPserver *ftpServer, char *OName)
{
    if (ftpServer->status == start || ftpServer->status == waitPass)
    {
        CheckWrite(ftpServer->cmdConnection.conn_fd, "530 Not logged in.\r\n", 20);
        ftpServer->status = start;
        return;
    }
    if (strlen(OName) == 0)
    {
        CheckWrite(ftpServer->cmdConnection.conn_fd, "501 Syntax error.\r\n", 19);
        return;
    }
    else
    {
        char targetFile[1200];
        sprintf(targetFile, "%s%s/%s", ftpServer->root_path, ftpServer->work_directory, OName);
        if (access(targetFile, F_OK) != 0)
        {
            CheckWrite(ftpServer->cmdConnection.conn_fd, "550 File unavailable.\r\n", 23);
            return;
        }
        else
        {
            strcpy(ftpServer->OName, OName);
            CheckWrite(ftpServer->cmdConnection.conn_fd, "350 Requested file action pending further information.\r\n", 56);
            ftpServer->status = reName;
        }
    }
}

void handle_RNTO(struct FTPserver *ftpServer, char *NName)
{
    if (ftpServer->status == start || ftpServer->status == waitPass)
    {
        CheckWrite(ftpServer->cmdConnection.conn_fd, "530 Not logged in.\r\n", 20);
        ftpServer->status = start;
        return;
    }
    else if (ftpServer->status == loggedIn)
    {
        CheckWrite(ftpServer->cmdConnection.conn_fd, "503 Bad sequence of commands.\r\n", 30);
        return;
    }
    else
    {
        char targetFile[1280];
        char targetName[1280];
        sprintf(targetFile, "%s%s/%s", ftpServer->root_path, ftpServer->work_directory, ftpServer->OName);
        sprintf(targetName, "%s%s/%s", ftpServer->root_path, ftpServer->work_directory, NName);
        //printf("new name: %s\r\n", targetName);
        if (rename(targetFile, targetName) == 0)
        {
            CheckWrite(ftpServer->cmdConnection.conn_fd, "250 Requested file action okay, file renamed.\r\n", 47);
            ftpServer->status = loggedIn;
        }
        else
        {
            CheckWrite(ftpServer->cmdConnection.conn_fd, "553 Cannot rename file.\r\n", 25);
            ftpServer->status = loggedIn;
        }
    }
}

void handle_STOR(struct FTPserver *ftpServer, char *fileName)
{
    if (ftpServer->status == start || ftpServer->status == waitPass)
    {
        CheckWrite(ftpServer->cmdConnection.conn_fd, "530 Not logged in.\r\n", 20);
        ftpServer->status = start;
        return;
    }
    if (ftpServer->mode == 0)
    {
        CheckWrite(ftpServer->cmdConnection.conn_fd, "PASV or PORT mode needed.\r\n", 27);
        return;
    }
    else if (ftpServer->mode == 1)
    {
        int conn_fd;
        if ((conn_fd = accept(ftpServer->dataConnection.conn_fd, (struct sockaddr *)NULL, NULL)) == -1)
        {
            //printf("accept socket error: %s\n\a", strerror(errno));
        }
        ftpServer->pasv_fd = conn_fd;
        CheckWrite(ftpServer->cmdConnection.conn_fd, "150 Opening data connection.\r\n", 30);

        struct transferInfo transfer_info;
        transfer_info.conn_fd = ftpServer->pasv_fd;
        transfer_info.cmd_fd = ftpServer->cmdConnection.conn_fd;
        transfer_info.startPlace = 0;
        strcpy(transfer_info.name, fileName);

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, (void *)(&fileGet), (void *)(&transfer_info)) == -1) {
            //printf("pthread create error: %s\n\a", strerror(errno));
            return;
        }        
        close(ftpServer->dataConnection.conn_fd);
    }
    else if (ftpServer->mode == 2)
    {
        if ((ftpServer->dataConnection.conn_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            fprintf(stderr, "Socket error:%s\n\a", strerror(errno));
            exit(1);
        }
        // 填充sockaddr_in结构
        struct sockaddr_in *data_addr = &ftpServer->dataConnection.addr;
        bzero(data_addr, sizeof(struct sockaddr_in));
        data_addr->sin_family = AF_INET;
        data_addr->sin_addr.s_addr = inet_addr(ftpServer->port_ip);
        data_addr->sin_port = htons(ftpServer->port_port);
        if (connect(ftpServer->dataConnection.conn_fd, (struct sockaddr *)data_addr, sizeof(struct sockaddr)) == -1)
        {
            fprintf(stderr, "Connect error:%s\n\a", strerror(errno));
            exit(1);
        }
        else
        {
            CheckWrite(ftpServer->cmdConnection.conn_fd, "150 Opening data connection.\r\n", 30);
        }

        struct transferInfo transfer_info;
        transfer_info.conn_fd = ftpServer->dataConnection.conn_fd;
        transfer_info.cmd_fd = ftpServer->cmdConnection.conn_fd;
        transfer_info.startPlace = 0;
        strcpy(transfer_info.name, fileName);

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, (void *)(&fileGet), (void *)(&transfer_info)) == -1) {
            //printf("pthread create error: %s\n\a", strerror(errno));
            return;
        }     
        ftpServer->dataConnection.valid = false;
    }
    ftpServer->mode = 0;
}

void handle_SYST(struct FTPserver *ftpServer)
{
    CheckWrite(ftpServer->cmdConnection.conn_fd, "215 UNIX Type: L8\r\n", 19);
}

void handle_TYPE(struct FTPserver *ftpServer, char *operand)
{
    if (!strcmp(operand, "I"))
    {
        CheckWrite(ftpServer->cmdConnection.conn_fd, "200 Type set to I.\r\n", 20);
    }
    else
    {
        CheckWrite(ftpServer->cmdConnection.conn_fd, "501 Syntax error.\r\n", 19);
    }
}

void handle_APPE(struct FTPserver* ftpServer, char* fileName) {
    if (ftpServer->status == start || ftpServer->status == waitPass)
    {
        CheckWrite(ftpServer->cmdConnection.conn_fd, "530 Not logged in.\r\n", 20);
        ftpServer->status = start;
        return;
    }
    if (ftpServer->mode == 0)
    {
        CheckWrite(ftpServer->cmdConnection.conn_fd, "PASV or PORT mode needed.\r\n", 27);
        return;
    }
    else if (ftpServer->mode == 1)
    {
        int conn_fd;
        if ((conn_fd = accept(ftpServer->dataConnection.conn_fd, (struct sockaddr *)NULL, NULL)) == -1)
        {
            //printf("accept socket error: %s\n\a", strerror(errno));
        }
        ftpServer->pasv_fd = conn_fd;
        CheckWrite(ftpServer->cmdConnection.conn_fd, "150 Opening data connection.\r\n", 30);
        //different from stor
        FILE* fp;
        int restPlace = 0;
        if(access(fileName, F_OK) == 0 && access(fileName, W_OK) == 0) {
            fp = fopen(fileName, "rb");
            fseek(fp, 0, SEEK_END);
            restPlace = ftell(fp);
        }

        struct transferInfo transfer_info;
        transfer_info.conn_fd = ftpServer->pasv_fd;
        transfer_info.cmd_fd = ftpServer->cmdConnection.conn_fd;
        transfer_info.startPlace = restPlace;
        strcpy(transfer_info.name, fileName);
        // different from stor
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, (void *)(&fileGet), (void *)(&transfer_info)) == -1) {
            //printf("pthread create error: %s\n\a", strerror(errno));
            return;
        }        
        close(ftpServer->dataConnection.conn_fd);
    }
    else if (ftpServer->mode == 2)
    {
        if ((ftpServer->dataConnection.conn_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            fprintf(stderr, "Socket error:%s\n\a", strerror(errno));
            exit(1);
        }
        // 填充sockaddr_in结构
        struct sockaddr_in *data_addr = &ftpServer->dataConnection.addr;
        bzero(data_addr, sizeof(struct sockaddr_in));
        data_addr->sin_family = AF_INET;
        data_addr->sin_addr.s_addr = inet_addr(ftpServer->port_ip);
        data_addr->sin_port = htons(ftpServer->port_port);
        if (connect(ftpServer->dataConnection.conn_fd, (struct sockaddr *)data_addr, sizeof(struct sockaddr)) == -1)
        {
            fprintf(stderr, "Connect error:%s\n\a", strerror(errno));
            exit(1);
        }
        else
        {
            CheckWrite(ftpServer->cmdConnection.conn_fd, "150 Opening data connection.\r\n", 30);
        }

        FILE* fp;
        int restPlace = 0;
        if(access(fileName, F_OK) == 0 && access(fileName, W_OK) == 0) {
            fp = fopen(fileName, "rb");
            fseek(fp, 0, SEEK_END);
            restPlace = ftell(fp);
        }

        struct transferInfo transfer_info;
        transfer_info.conn_fd = ftpServer->dataConnection.conn_fd;
        transfer_info.cmd_fd = ftpServer->cmdConnection.conn_fd;
        transfer_info.startPlace = restPlace;
        strcpy(transfer_info.name, fileName);

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, (void *)(&fileGet), (void *)(&transfer_info)) == -1) {
            //printf("pthread create error: %s\n\a", strerror(errno));
            return;
        }     
        ftpServer->dataConnection.valid = false;
    }
    ftpServer->mode = 0;
}

void requestHandle(struct FTPrequest request, int conn_fd, struct FTPserver *ftpServer)
{
    if (request.type == UNDEFINED)
    {
        send(conn_fd, "Unknown Request Command\r\n", 27, 0);
    }
    else if (request.type == USER)
    {
        handle_USER(ftpServer, request.operand, conn_fd);
    }
    else if (request.type == CWD)
    {
        handle_CWD(ftpServer, request.operand);
    }
    else if (request.type == PWD)
    {
        handle_PWD(ftpServer);
    }
    else if (request.type == PASS)
    {
        handle_PASS(ftpServer, request.operand);
    }
    else if (request.type == PASV)
    {
        handle_PASV(ftpServer);
    }
    else if (request.type == PORT)
    {
        handle_PORT(ftpServer, request.operand);
    }
    else if (request.type == RETR)
    {
        handle_RETR(ftpServer, request.operand);
    }
    else if (request.type == RNFR)
    {
        handle_RNFR(ftpServer, request.operand);
    }
    else if (request.type == RNTO)
    {
        handle_RNTO(ftpServer, request.operand);
    }
    else if (request.type == STOR)
    {
        handle_STOR(ftpServer, request.operand);
    }
    else if (request.type == QUIT)
    {
        handle_QUIT(ftpServer);
    }
    else if (request.type == LIST)
    {
        handle_LIST(ftpServer);
    }
    else if (request.type == MKD)
    {
        handle_MKD(ftpServer, request.operand);
    }
    else if (request.type == RMD)
    {
        handle_RMD(ftpServer, request.operand);
    }
    else if (request.type == SYST)
    {
        handle_SYST(ftpServer);
    }
    else if (request.type == TYPE)
    {
        handle_TYPE(ftpServer, request.operand);
    }
    else if (request.type == REST) {
        handle_REST(ftpServer, request.operand);
    }
    else if (request.type == APPE) {
        handle_APPE(ftpServer, request.operand);
    }
}

int generateRandPort()
{
    srand((unsigned int)time(NULL));
    return rand() % 50000 + 1024;
}