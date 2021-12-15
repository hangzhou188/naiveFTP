#ifndef QTCLIENT_H
#define QTCLIENT_H
#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTcpServer>
#include <QMessageBox>
enum cmdType{
    APPE,
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
    DELE,
    UNDEFINED,
    START
};

enum clientStatus {
    notconnected,
    notlogin,
    loggedin
};

class QtClient : public QObject
{
    Q_OBJECT
public:
    explicit QtClient(QObject *parent = nullptr);
    QTcpSocket* cmdSocket;
    QTcpSocket *dataSocket;
    QTcpServer * dataSocket_PORT;
    QString SeverIP;
    clientStatus status;
    char retBuff[1025];
    char dataBuff[8192];
    unsigned short PasvPort;
    cmdType curType;
    void connectToServer(QString serverIP, unsigned short serverPort);
    void readCmdBuff();
    void statusChange();
    void openPasvLink();
    void sendCmd(cmdType type, const char* operand);
    bool listenPort(QString IP, unsigned short port);
    void triggerConnectError();
    void EmitDisconnect();
signals:
    void connectError();
    void disconnected();
    void completeRead(cmdType type);
public slots:
};

#endif // QTCLIENT_H
