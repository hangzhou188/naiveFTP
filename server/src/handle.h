#include "serverfunction.h"
#ifndef HANDLE_H
#define HANDLE_H

#define BUF_SIZE 512

enum requestType
{
    USER,
    PASS,
    CWD,
    PWD,
    PORT,
    PASV,
    RETR,
    STOR,
    LIST,
    MKD,
    RMD,
    RNFR,
    RNTO,
    REST,
    SYST,
    TYPE,
    QUIT,
    APPE,
    UNDEFINED
};

struct FTPrequest
{
    enum requestType type;
    char operand[128];
};

char *trimSpace(char *buff);

void trimBackSlash(char* buff);

void transferIP(char* dest, char* src);

bool seekSplit(char *dest, char *resc, int place);

bool splitBefore(char *dest, char *resc, int place);

bool isPositiveNumber(char* m_str);

struct FTPrequest parseBuff(char *buff);

void handle_USER(struct FTPserver *ftpServer, char *userName, int conn_fd);

void handle_PASS(struct FTPserver *ftpServer, char *passWord);

void handle_CWD(struct FTPserver *ftpServer, char *path);

void handle_PWD(struct FTPserver *ftpServer);

void handle_PASV(struct FTPserver *ftpServer);

void handle_LIST(struct FTPserver *ftpServer);

void handle_PORT(struct FTPserver *ftpServer, char *ip_port);

void handle_RETR(struct FTPserver *ftpServer, char *filePath);

void handle_STOR(struct FTPserver *ftpServer, char *fileName);

void handle_SYST(struct FTPserver *ftpServer);

void handle_TYPE(struct FTPserver *ftpServer, char *operand);

void handle_RNFR(struct FTPserver *ftpServer, char *OName);

void handle_RNTO(struct FTPserver *ftpServer, char *NName);

void handle_MKD(struct FTPserver *ftpServer, char *DName);

void handle_REST(struct FTPserver* ftpServer, char* restPlaceSTR);

void handle_RMD(struct FTPserver *ftpServer, char *DName);

void handle_QUIT(struct FTPserver *ftpServer);

void handle_APPE(struct FTPserver* ftpServer, char* operand);

void requestHandle(struct FTPrequest request, int conn_fd, struct FTPserver *ftpServer);

int generateRandPort();
#endif