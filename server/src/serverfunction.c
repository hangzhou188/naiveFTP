#include "serverfunction.h"

void fileTransfer(void* transfer_info)
{
    struct transferInfo Info = *((struct transferInfo*) transfer_info);
    FILE *fp;
    //printf("transferring...\r\n");
    int conn_fd = Info.conn_fd;
    if ((fp = fopen(Info.name, "rb")) == NULL)
    {
        fprintf(stderr, "File open error: %s,%s\n\a", strerror(errno), Info.name);
        close(conn_fd);
        CheckWrite(Info.cmd_fd, "550 No such File.\r\n",19);
        return;
    }
    fseek(fp, 0, SEEK_END);
    int fileSize = ftell(fp);
    if(fileSize < Info.startPlace) {
        CheckWrite(Info.cmd_fd, "550 REST place out of file range.\r\n", 35);
        return;
    } else {
        fseek(fp, Info.startPlace, SEEK_SET);
    }
    //printf ("start place: %d\r\n", Info.startPlace);
    char buff[1025];
    while (!feof(fp))
    {
        int len = fread(buff, sizeof(char), 1024, fp);
        CheckWrite(conn_fd, buff, len);
        bzero(buff, sizeof(buff));
    }
    //printf("complete transferring\r\n");
    CheckWrite(Info.cmd_fd, "226 Transfer complete.\r\n", 24);
    close(conn_fd);
    fclose(fp);
    pthread_exit(NULL);
}

void fileGet(void * transfer_info)
{
    struct transferInfo Info = *((struct transferInfo*) transfer_info);
    FILE *fp;
    if ((fp = fopen(Info.name, "wb")) == NULL)
    {
        //printf("File open error\r\n");
        CheckWrite(Info.cmd_fd, "550 Cannot STOR file.\r\n", 23);
        return;
    }
    char buff[1025];
    int retStatus;
    int conn_fd = Info.conn_fd;
    int count = 0;
    
    if (Info.startPlace != 0) {
        fseek(fp, Info.startPlace, SEEK_SET);
    }
    
    while (1)
    {
        bzero(buff, sizeof(buff));
        retStatus = recv(conn_fd, buff, 1024, 0);
        count += retStatus;
        if (retStatus == 0)
        {
            break;
        }
        else if (retStatus < 0)
        {
            //printf("RECV error: %s\r\n", strerror(errno));
            break;
        }
        fwrite(buff, sizeof(char), retStatus, fp);
    }
    fclose(fp);
    CheckWrite(Info.cmd_fd, "226 Transfer complete.\r\n", 24);
    close(conn_fd);
    pthread_exit(NULL);
}

void CheckWrite(int conn_fd, char *buff, size_t len)
{
    if (send(conn_fd, buff, len, 0) == -1)
    {
        fprintf(stderr, "Message send error:%s\n\a", strerror(errno));
    }
}