#include "qtclient.h"

QtClient::QtClient(QObject *parent) : QObject(parent)
{
    cmdSocket = nullptr;
    curType = START;
    dataSocket = nullptr;
    dataSocket_PORT = nullptr;
    status = notconnected;
}

void QtClient::connectToServer(QString serverIP, unsigned short serverPort) {
    if (cmdSocket) {
        cmdSocket->close();
    }
    cmdSocket = new QTcpSocket(this);
    try {
        cmdSocket->connectToHost(serverIP, serverPort);
    } catch (QString &error) {
        qDebug()<<error;
        return;
    }
    if( cmdSocket->waitForReadyRead(5000) == true){
        readCmdBuff();
    } else {
        QMessageBox::information(nullptr, "Warning", "cannot connect to server");
        //qDebug()<<"can't connect";
    }
    this->SeverIP = serverIP;
    connect(cmdSocket, &QTcpSocket::disconnected, this, &QtClient::EmitDisconnect);
    connect(cmdSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &QtClient::connectError);
}

void QtClient::readCmdBuff() {
   memset(retBuff, 0, sizeof(retBuff));
   cmdSocket->read(retBuff, 1024);
   statusChange();
   //cmdSocket->waitForReadyRead(100);
   emit completeRead(curType);
}

void QtClient::sendCmd(cmdType type,const char *operand) {
    char buff[1024];
    long long iResult;
    memset(retBuff,0 , sizeof(retBuff));
    curType = type;
    if (type == USER) {
        sprintf(buff, "USER %s\r\n",operand);
        iResult = cmdSocket->write(buff, strlen(buff));
        cmdSocket->flush();
        if(iResult < 0) {
            sprintf(retBuff, "Failed to send command %s\r\n.", buff);
            return;
        }
        if( cmdSocket->waitForReadyRead(5000) == true){
            readCmdBuff();
        }
    }
    else if (type == PASS) {
        sprintf(buff, "PASS %s\r\n",operand);
        iResult = cmdSocket->write(buff, strlen(buff));
        cmdSocket->flush();
        if(iResult < 0) {
            sprintf(retBuff, "Failed to send command %s\r\n.", buff);
            return;
        }
        if( cmdSocket->waitForReadyRead(5000) == true){
            readCmdBuff();
            if(retBuff[0] == '2' &&retBuff[1] == '3' && retBuff[2] == '0'){
                status = loggedin;
            }
        }
    }
    else if (type == PASV) {
        iResult = cmdSocket->write("PASV\r\n", 6);
        cmdSocket->flush();
        if(iResult < 0) {
            sprintf(retBuff, "Failed to send command PASV\r\n.");
            return;
        }
        if( cmdSocket->waitForReadyRead(5000) == true){
            readCmdBuff();
        }
    }
    else if (type == LIST) {
        iResult = cmdSocket->write("LIST\r\n", 6);
        cmdSocket->flush();
        if(iResult < 0) {
            sprintf(retBuff, "Failed to send command LIST\r\n.");
            return;
        }
    }
    else if (type == PORT) {
        sprintf(buff, "PORT %s\r\n",operand);
        iResult = cmdSocket->write(buff, strlen(buff));
        cmdSocket->flush();
        if(iResult < 0) {
            sprintf(retBuff, "Failed to send command PORT\r\n.");
            return;
        }
        if( cmdSocket->waitForReadyRead(5000) == true){
            readCmdBuff();
        } else {
            qDebug()<<"time out";
        }
    }
    else if (type == PWD) {
        iResult = cmdSocket->write("PWD\r\n", 5);
        cmdSocket->flush();
        if(iResult < 0) {
            sprintf(retBuff, "Failed to send command PWD\r\n.");
            return;
        }
        if( cmdSocket->waitForReadyRead(5000) == true){
            readCmdBuff();
        }
    }
    else if (type == CWD) {
        sprintf(buff, "CWD %s\r\n",operand);
        iResult = cmdSocket->write(buff, strlen(buff));
        cmdSocket->flush();
        if(iResult < 0) {
            sprintf(retBuff, "Failed to send command %s\r\n.", buff);
            return;
        }
    }
    else if (type == MKD) {
        sprintf(buff, "MKD %s\r\n",operand);
        iResult = cmdSocket->write(buff, strlen(buff));
        cmdSocket->flush();
        if(iResult < 0) {
            sprintf(retBuff, "Failed to send command %s\r\n.", buff);
            return;
        }
    }
    else if (type == RNFR) {
        sprintf(buff, "RNFR %s\r\n",operand);
        iResult = cmdSocket->write(buff, strlen(buff));
        cmdSocket->flush();
        if(iResult < 0) {
            sprintf(retBuff, "Failed to send command %s\r\n.", buff);
            return;
        }
    }
    else if (type == RNTO) {
        sprintf(buff, "RNTO %s\r\n",operand);
        iResult = cmdSocket->write(buff, strlen(buff));
        cmdSocket->flush();
        if(iResult < 0) {
            sprintf(retBuff, "Failed to send command %s\r\n.", buff);
            return;
        }
    }
    else if (type == QUIT) {
        iResult = cmdSocket->write("QUIT\r\n", 6);
        cmdSocket->flush();
        if(iResult < 0) {
            sprintf(retBuff, "Failed to send command QUIT\r\n.");
            return;
        }
        if( cmdSocket->waitForReadyRead(5000) == true){
            readCmdBuff();
        }
    }
    else if (type == RETR) {
        sprintf(buff, "RETR %s\r\n",operand);
        iResult = cmdSocket->write(buff, strlen(buff));
        cmdSocket->flush();
        if(iResult < 0) {
            sprintf(retBuff, "Failed to send command %s\r\n.", buff);
            return;
        }
    }
    else if (type == SYST) {
        iResult = cmdSocket->write("SYST\r\n", 6);
        cmdSocket->flush();
        if(iResult < 0) {
            sprintf(retBuff, "Failed to send command SYST\r\n.");
            return;
        }
        if( cmdSocket->waitForReadyRead(5000) == true){
            readCmdBuff();
        }
    }
    else if (type == STOR) {
        sprintf(buff, "STOR %s\r\n",operand);
        iResult = cmdSocket->write(buff, strlen(buff));
        cmdSocket->flush();
        if(iResult < 0) {
            sprintf(retBuff, "Failed to send command %s\r\n.", buff);
            return;
        }
    }
    else if (type == TYPE) {
        iResult = cmdSocket->write("TYPE I\r\n", 8);
        cmdSocket->flush();
        if(iResult < 0) {
            sprintf(retBuff, "Failed to send command TYPE\r\n.");
            return;
        }
        if( cmdSocket->waitForReadyRead(5000) == true){
            readCmdBuff();
        }
    }
    else if (type == APPE) {
        sprintf(buff, "APPE %s\r\n",operand);
        iResult = cmdSocket->write(buff, strlen(buff));
        cmdSocket->flush();
        if(iResult < 0) {
            sprintf(retBuff, "Failed to send command %s\r\n.", buff);
            return;
        }
    }
    else if (type == DELE) {
        sprintf(buff, "DELE %s\r\n",operand);
        iResult = cmdSocket->write(buff, strlen(buff));
        cmdSocket->flush();
        if(iResult < 0) {
            sprintf(retBuff, "Failed to send command %s\r\n.", buff);
            return;
        }
    }
    else if (type == RMD) {
        sprintf(buff, "RMD %s\r\n",operand);
        iResult = cmdSocket->write(buff, strlen(buff));
        cmdSocket->flush();
        if(iResult < 0) {
            sprintf(retBuff, "Failed to send command %s\r\n.", buff);
            return;
        }
    }
    else if (type == REST) {
        sprintf(buff, "REST %s\r\n",operand);
        iResult = cmdSocket->write(buff, strlen(buff));
        cmdSocket->flush();
        if(iResult < 0) {
            sprintf(retBuff, "Failed to send command %s\r\n.", buff);
            return;
        }
    }
}

void QtClient::statusChange() {
    if(curType == QUIT && (retBuff[0] == '2' &&retBuff[1] == '2' && retBuff[2] == '1')) {
        //cmdSocket->close();
        status = notconnected;
    } else if((curType == START || curType == QUIT) && retBuff[0] == '2' && retBuff[1] == '2' && retBuff[2] == '0') {
        status = notlogin;
    }
}

void QtClient::openPasvLink() {
    if(dataSocket) {
        dataSocket->close();
    }
    dataSocket = new QTcpSocket();
    dataSocket->connectToHost(SeverIP, PasvPort);
    if(dataSocket->waitForConnected(5000) == true) {
        //qDebug()<<"connected";
    }
}

void QtClient::triggerConnectError() {
    qDebug()<<cmdSocket->errorString();
    emit(this->connectError());
}

void QtClient::EmitDisconnect() {
    emit disconnected();
}

bool QtClient::listenPort(QString IP, unsigned short port) {
    if (dataSocket) {
        dataSocket->close();
    }
    if(dataSocket_PORT) {
        dataSocket_PORT->close();
    }
    dataSocket_PORT = new QTcpServer(this);
    if (dataSocket_PORT->listen(QHostAddress(IP), port)){
        return true;
    } else {
        return false;
    }
}

