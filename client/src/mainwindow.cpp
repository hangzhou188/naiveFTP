#include "mainwindow.h"
#include "ui_mainwindow.h"
#pragma execution_character_set("utf-8")

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(QDesktopWidget().availableGeometry(this).size() * 0.8);
    // init the elements
    label_host = new QLabel(this);
    label_mode = new QLabel(this);
    label_port = new QLabel(this);
    label_password = new QLabel(this);
    label_username = new QLabel(this);

    line_host = new QLineEdit(this);
    line_port = new QLineEdit(this);
    line_username = new QLineEdit(this);
    line_password = new QLineEdit(this);
    line_password->setEchoMode(QLineEdit::Password);
    line_curpath = new QLineEdit(this);
    line_curpath->setReadOnly(true);

    button_PASV = new QRadioButton(this);
    button_PORT = new QRadioButton(this);

    push_quit = new QPushButton(this);
    push_connect = new QPushButton(this);
    push_login = new QPushButton(this);
    push_fresh = new QPushButton(this);
    push_mkdir = new QPushButton(this);
    push_download = new QPushButton(this);
    push_upload = new QPushButton(this);
    push_rename = new QPushButton(this);
    push_append = new QPushButton(this);
    push_rest = new QPushButton(this);
    push_delete = new QPushButton(this);
    push_rmd = new QPushButton(this);

    centralWidget = new QWidget(this);

    mainLayout = new QGridLayout();
    topLayout = new QHBoxLayout();
    secondLayout = new QVBoxLayout();
    second_topLayout = new QHBoxLayout();
    second_secondLayout = new QHBoxLayout();
    second_thirdLayout = new QHBoxLayout();

    fileTable = new QTableWidget(this);
    fileTable->setColumnCount(4);
    fileTable->setRowCount(1);
    fileTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    fileTable->setSelectionMode(QAbstractItemView::SingleSelection);
    fileTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    QStringList tableHeader;
    tableHeader<<"文件名"<<"文件类型"<<"文件大小"<<"最近修改时间";
    fileTable->setHorizontalHeaderLabels(tableHeader);
    ioWidget = new QTextEdit(this);
    ioWidget->setReadOnly(true);
    //set central widget and its layout
    this -> setCentralWidget(centralWidget);
    mainLayout->addLayout(topLayout, 0, 0);
    mainLayout->addLayout(secondLayout,1,0);
    mainLayout->addWidget(ioWidget, 2,0);

    centralWidget->setLayout(mainLayout);
    // place the top labels and forms
    label_host->setText("Host:");
    label_port->setText("Port:");
    label_mode->setText("Mode:");
    label_username->setText("UserName:");
    label_password->setText("Password:");

    button_PASV->setText("PASV");
    button_PORT->setText("PORT");
    button_PORT->setChecked(true);

    push_connect->setText("Connect");
    push_login->setText("Login");
    push_quit->setText("Quit");
    push_mkdir->setText("新建文件夹");
    push_fresh->setText("刷新");
    push_download->setText("下载");
    push_upload->setText("上传");
    push_rename->setText("重命名");
    push_append->setText("断点上传");
    push_rest->setText("断点下载");
    push_delete->setText("删除文件");
    push_rmd->setText("删除文件夹");

    topLayout->addWidget(label_host);
    topLayout->addWidget(line_host);
    topLayout->addWidget(label_username);
    topLayout->addWidget(line_username);
    topLayout->addWidget(label_password);
    topLayout->addWidget(line_password);
    topLayout->addWidget(label_port);
    topLayout->addWidget(line_port);
    topLayout->addWidget(label_mode);
    topLayout->addWidget(button_PORT);
    topLayout->addWidget(button_PASV);
    topLayout->addWidget(push_connect);
    topLayout->addWidget(push_login);
    topLayout->addWidget(push_quit);

    secondLayout->addLayout(second_topLayout);
    secondLayout->addLayout(second_secondLayout);
    secondLayout->addLayout(second_thirdLayout);
    secondLayout->addWidget(fileTable);

    second_topLayout->addWidget(push_fresh);
    second_topLayout->addWidget(push_mkdir);
    second_topLayout->addWidget(push_upload);
    second_topLayout->addWidget(push_download);
    second_topLayout->addWidget(push_rename);

    second_secondLayout->addWidget(push_append);
    second_secondLayout->addWidget(push_rest);
    second_secondLayout->addWidget(push_delete);
    second_secondLayout->addWidget(push_rmd);

    second_thirdLayout->addWidget(line_curpath);

    //set button conditions
    push_login->setEnabled(false);
    push_quit->setEnabled(false);
    push_fresh->setEnabled(false);
    push_rename->setEnabled(false);
    push_mkdir->setEnabled(false);
    push_upload->setEnabled(false);
    push_download->setEnabled(false);
    push_rest->setEnabled(false);
    push_delete->setEnabled(false);
    push_rmd->setEnabled(false);
    push_append->setEnabled(false);

    // events
    connect(push_connect, &QPushButton::clicked, this, &MainWindow::connectToServer);
    connect(push_login, &QPushButton::clicked, this, &MainWindow::logIn);
    connect(push_quit, &QPushButton::clicked, this, &MainWindow::quit);
    connect(push_fresh, &QPushButton::clicked, this, &MainWindow::refresh);
    connect(push_download, &QPushButton::clicked, this, &MainWindow::download);
    connect(push_upload, &QPushButton::clicked, this, &MainWindow::upload);
    connect(push_mkdir, &QPushButton::clicked, this, &MainWindow::onMKD);
    connect(push_rename, &QPushButton::clicked, this, &MainWindow::onRename);
    connect(push_append, &QPushButton::clicked, this, &MainWindow::append);
    connect(push_rest, &QPushButton::clicked, this, &MainWindow::rest);
    connect(push_delete, &QPushButton::clicked, this, &MainWindow::deleteFile);
    connect(push_rmd, &QPushButton::clicked, this, &MainWindow::onRMD);
    connect(&qtClient, SIGNAL(completeRead(cmdType)), this,
            SLOT(handleRetBuff(cmdType)));
    //disconnect
    connect(&qtClient, SIGNAL(disconnected()), this, SLOT(onSocketDisconnect()));
    connect(fileTable, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(onTableItemClicked(int, int)));
}

MainWindow::~MainWindow()
{
    // pay attention to the delete order
    delete second_topLayout;
    delete second_secondLayout;
    delete topLayout;
    delete secondLayout;
    delete mainLayout;
    delete ui;
}

void MainWindow::connectToServer() {
    std::string serverIP_string = line_host->text().toStdString();
    const char* serverIP = serverIP_string.c_str();
    unsigned short serverPort = line_port->text().toUShort();
    qtClient.connectToServer(serverIP, serverPort);
}

void MainWindow::logIn() {
    std::string userName_stdString = line_username->text().toStdString();
    const char* userName = userName_stdString.c_str();
    qtClient.sendCmd(USER, userName);
    std::string password_stdString = line_password->text().toStdString();
    const char* password = password_stdString.c_str();
    qtClient.sendCmd(PASS, password);
    if (qtClient.status == loggedin) {
        qtClient.sendCmd(SYST, "");
        qtClient.sendCmd(TYPE, "");
        refresh();
    } else {

    }
}

void MainWindow::quit() {
    qtClient.sendCmd(QUIT, "");
}

void MainWindow::askForSYST() {
    qtClient.sendCmd(SYST, "");
}

void MainWindow::askForPWD() {
    qtClient.sendCmd(PWD, "");
}

void MainWindow::typeBinary() {
    qtClient.sendCmd(TYPE, "");
}

void MainWindow::refresh() {
    qtClient.sendCmd(PWD,"");
    PortOK = false;
    if (button_PASV->isChecked()) {
        qtClient.sendCmd(PASV,"");
        qtClient.sendCmd(LIST, "");
        qtClient.openPasvLink();
        waitNread();
    } else {
        Port();
        if(!PortOK) {
            QMessageBox::warning(this, "warning", "failed to build Port connection");
            return;
        } else {
            qtClient.sendCmd(LIST, "");
            //先握手再建立连接
            waitNread();
            if (qtClient.dataSocket_PORT->waitForNewConnection(5000)) {
                 qtClient.dataSocket = (qtClient.dataSocket_PORT->nextPendingConnection());
            } else {
                QMessageBox::information(this, "Warning", qtClient.dataSocket_PORT->errorString());
                return;
            }
        }
    }
    std::string listResult;
    QString qt_listResult;
    while(qtClient.dataSocket->waitForReadyRead(5000)) {
        qt_listResult += qtClient.dataSocket->readAll();
    }
    qtClient.dataSocket->close();
    if( qtClient.cmdSocket->waitForReadyRead(1500) == true){
        qtClient.readCmdBuff();
    }
    qt_listResult.replace("\r\n","\n");
    QStringList qt_listResult_list = qt_listResult.split("\n");
    qt_listResult_list.pop_back();
    fileTable->clearContents();
    fileTable->setRowCount(qt_listResult_list.size()+1);
    QTableWidgetItem *item0 = new QTableWidgetItem("..");
    fileTable->setItem(0,0,item0);
    QTableWidgetItem *item1 = new QTableWidgetItem("文件夹");
    fileTable->setItem(0,1, item1);
    int row = 1;
    QStringList qt_listResult_sorted;
    foreach(QString fileinfo ,qt_listResult_list) {
        if (fileinfo[0] == 'd') {
            qt_listResult_sorted.push_back(fileinfo);
        }
    }
    foreach(QString fileinfo ,qt_listResult_list) {
        if (fileinfo[0] != 'd') {
            qt_listResult_sorted.push_back(fileinfo);
        }
    }
    foreach(QString fileinfo, qt_listResult_sorted) {
        fileinfo.replace(QRegExp("[\\s]+"), " ");
        QStringList fileinfo_split = fileinfo.split(" ");
        QTableWidgetItem *item0 = new QTableWidgetItem(fileinfo_split[8]);
        fileTable->setItem(row, 0, item0);

        QString text;
        if(fileinfo_split[0][0] == 'd') {
            text = "文件夹";
        } else {
            text = "文件";
        }
        QTableWidgetItem *item1 = new QTableWidgetItem(text);
        fileTable->setItem(row, 1, item1);

        QTableWidgetItem *item2 = new QTableWidgetItem(fileinfo_split[4]);
        fileTable->setItem(row, 2, item2);

        QTableWidgetItem *item3 = new QTableWidgetItem(fileinfo_split[5] + " " + fileinfo_split[6] + " " + fileinfo_split[7]);
        fileTable->setItem(row, 3, item3);

        ++row;
    }
}

void MainWindow::download() {
    //PASV MODE Download
    if(button_PASV->isChecked()) {
        qtClient.sendCmd(PASV,"");
        QList<QTableWidgetItem*> selected = fileTable->selectedItems();
        if (selected.length()<=2 || selected[1]->text() != "文件") {
            QMessageBox::information(this, "提示", "请选择一个文件");
            return;
        }
        //qDebug()<<selected[0]->text();
        QString fileName = QFileDialog::getSaveFileName(this,"另存为","./"+selected[0]->text());
        if(!fileName.length()) {
            return;
        }
        QFile file(fileName);
        file.open(QIODevice::WriteOnly);
        qtClient.sendCmd(RETR, selected[0]->text().toStdString().c_str());
        qtClient.openPasvLink();
        //qtClient.dataSocket->moveToThread(m_thread);
        if( qtClient.cmdSocket->waitForReadyRead(5000) == true){
            qtClient.readCmdBuff();
        }


        while(qtClient.dataSocket->waitForReadyRead(5000)) {
            //qApp->processEvents();
            file.write(qtClient.dataSocket->readAll());
        }
        file.close();


        waitNread();
    }
    else if (button_PORT->isChecked()) {
        Port();
        QList<QTableWidgetItem*> selected = fileTable->selectedItems();
        if (selected.length()<=2 || selected[1]->text() != "文件") {
            QMessageBox::information(this, "提示", "请选择一个文件");
            return;
        }
        //qDebug()<<selected[0]->text();
        QString fileName = QFileDialog::getSaveFileName(this,"另存为","./"+selected[0]->text());
        if(!fileName.length()) {
            return;
        }
        QFile file(fileName);
        file.open(QIODevice::WriteOnly);
        qtClient.sendCmd(RETR, selected[0]->text().toStdString().c_str());
        if (!qtClient.dataSocket_PORT) {
            QMessageBox::information(this, "Warning", "Invalid datasocketPort");
            return;
        }
        if (qtClient.dataSocket_PORT->waitForNewConnection(5000)) {
             qtClient.dataSocket = (qtClient.dataSocket_PORT->nextPendingConnection());
        } else {
            QMessageBox::information(this, "Warning", qtClient.dataSocket_PORT->errorString());
            return;
        }
        if(qtClient.cmdSocket->waitForReadyRead(5000)){
            qtClient.readCmdBuff();
        }
        while (qtClient.dataSocket->waitForReadyRead(5000)) {
            //qApp->processEvents();
            file.write(qtClient.dataSocket->readAll());
        }
        if( qtClient.cmdSocket->waitForReadyRead(5000) == true){
            qtClient.readCmdBuff();
        }
        file.close();
        qtClient.dataSocket->close();
    }
}

void MainWindow::upload() {
    QString fileName = QFileDialog::getOpenFileName(this, "打开文件", "./");
    if(fileName.length() == 0) {
        return;
    }
    QString remoteName = fileName.split('/').last();
    if (button_PASV->isChecked()) {
        qtClient.sendCmd(PASV,"");
        qtClient.sendCmd(STOR, remoteName.toStdString().c_str());
        qtClient.openPasvLink();
        if( qtClient.cmdSocket->waitForReadyRead(5000) == true){
            qtClient.readCmdBuff();
        } else {
            QMessageBox::warning(this, "Warning","Time out");
            return;
        }
    } else {
        PortOK = false;
        Port();
        if(!PortOK) {
            QMessageBox::information(this, "warning", "failed to build port connection");
            return;
        } else {
            qtClient.sendCmd(STOR, remoteName.toStdString().c_str());
            if( qtClient.cmdSocket->waitForReadyRead(5000) == true){
                qtClient.readCmdBuff();
            } else {
                QMessageBox::warning(this, "Warning","Time out");
                return;
            }
            if (qtClient.dataSocket_PORT->waitForNewConnection(5000)) {
                 qtClient.dataSocket = (qtClient.dataSocket_PORT->nextPendingConnection());
            } else {
                QMessageBox::information(this, "Warning", qtClient.dataSocket_PORT->errorString());
                return;
            }
        }
    }
    // transfer the file
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    while(!file.atEnd()) {
        QByteArray segement = file.read(8192);
        qtClient.dataSocket->write(segement);
        qtClient.dataSocket->flush();
    }
    qtClient.dataSocket->close();
    if( qtClient.cmdSocket->waitForReadyRead(5000) == true){
        qtClient.readCmdBuff();
    } else {
        QMessageBox::warning(this, "Warning","Time out");
        return;
    }

    //
    refresh();
}

void MainWindow::handleRetBuff(cmdType type){
    ioWidget->append(qtClient.retBuff);
    QString retBuff(qtClient.retBuff);
    if (type == PWD) {
       int begin = retBuff.indexOf('\"');
       int end = retBuff.lastIndexOf('\"');
       line_curpath->setText(retBuff.mid(begin+1,end-begin-1));
    } else if(type == PASV) {
        QStringList buffList = retBuff.split(",");
        QString strp1 = buffList[4];
        QString strp2 = buffList[5];
        int end = strp2.lastIndexOf(')');
        unsigned short p1 = strp1.toUShort();
        unsigned short p2 = strp2.mid(0, end).toUShort();
        qtClient.PasvPort = p1 * 256 + p2;
        //qDebug()<<qtClient.PasvPort;
    } else if (type == PORT) {
        if (retBuff.startsWith("200")) {
            qtClient.listenPort(localIP, Port_port);
            PortOK = true;
        }
    } else if (type == PASS) {
        if (retBuff.startsWith("230")) {
            qtClient.status = loggedin;
            push_login->setEnabled(false);
        }
    } else if (type == CWD) {
        if (retBuff.startsWith("250")) {
            refresh();
        }
    }
    else if (type == QUIT) {
            if (retBuff.startsWith("221")) {
                qtClient.cmdSocket->close();
            }
        }
    QStringList retList = retBuff.split("\n");
    foreach(QString buff, retList) {
        if (buff.startsWith("5") || buff.startsWith("4")) {
            QMessageBox::warning(this, "Warning", buff);
        }
    }


    if(qtClient.status == notlogin) {
        push_connect->setEnabled(false);
        push_login->setEnabled(true);
        push_quit->setEnabled(false);
        push_fresh->setEnabled(false);
        push_rename->setEnabled(false);
        push_mkdir->setEnabled(false);
        push_upload->setEnabled(false);
        push_download->setEnabled(false);
        push_rest->setEnabled(false);
        push_delete->setEnabled(false);
        push_append->setEnabled(false);
        push_rmd->setEnabled(false);
    } else if(qtClient.status == notconnected){
        push_connect->setEnabled(true);
        push_login->setEnabled(false);
        push_quit->setEnabled(false);
        push_fresh->setEnabled(false);
        push_rename->setEnabled(false);
        push_mkdir->setEnabled(false);
        push_upload->setEnabled(false);
        push_download->setEnabled(false);
        push_rest->setEnabled(false);
        push_delete->setEnabled(false);
        push_append->setEnabled(false);
        push_rmd->setEnabled(false);
    } else {
        push_login->setEnabled(false);
        push_connect->setEnabled(false);
        push_quit->setEnabled(true);
        push_fresh->setEnabled(true);
        push_rename->setEnabled(true);
        push_mkdir->setEnabled(true);
        push_upload->setEnabled(true);
        push_download->setEnabled(true);
        push_rest->setEnabled(true);
        push_delete->setEnabled(true);
        push_rmd->setEnabled(true);
        push_append->setEnabled(true);
    }
}

void MainWindow::getLocalIP() {
    QString localHostName = QHostInfo::localHostName();
    QHostInfo localHostInfo = QHostInfo::fromName(localHostName);
    QHostAddress localAddress = qtClient.cmdSocket->localAddress();
    localIP = localAddress.toString();
    //foreach(QHostAddress address,localHostInfo.addresses()){
    //    if(address.protocol() == QAbstractSocket::IPv4Protocol && !(address.toString().startsWith("172.")))
    //    {
    //        localIP = address.toString();
    //        break;
    //    }
    //}
}

unsigned short MainWindow::generateRandPort() {
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    unsigned short port = qrand()%50000 + 1025;
    return port;
}

void MainWindow::Port() {
    getLocalIP();
    QString localIP_sendForm = localIP.replace('.',',');
    unsigned short randPort = generateRandPort();
    Port_port = randPort;
    localIP_sendForm = localIP_sendForm + ',' + QString::number(randPort/256) + ',' + QString::number(randPort % 256);
    qtClient.sendCmd(PORT, localIP_sendForm.toStdString().c_str());
}

void MainWindow::onSocketDisconnect() {
    ioWidget->append("Disconnected from remote  ftp server.\r\n");
    ioWidget->append(QString(32, '-'));
    qtClient.status = notconnected;
    fileTable->clearContents();
    line_curpath->setText("/");
    fileTable->setRowCount(0);
    push_connect->setEnabled(true);
    push_login->setEnabled(false);
    push_quit->setEnabled(false);
    push_fresh->setEnabled(false);
    push_rename->setEnabled(false);
    push_mkdir->setEnabled(false);
    push_upload->setEnabled(false);
    push_download->setEnabled(false);
    push_rest->setEnabled(false);
    push_delete->setEnabled(false);
    push_append->setEnabled(false);
}

void MainWindow::onTableItemClicked(int row, int column) {
    QList<QTableWidgetItem*> selected = fileTable->selectedItems();
    if(selected.length()>0 && selected[0]->text().length()) {
        if(selected.length()>1 && selected[1]->text() == "文件夹") {
            qtClient.sendCmd(CWD, selected[0]->text().toStdString().c_str());
            //wait for response
            if( qtClient.cmdSocket->waitForReadyRead(5000) == true){
                qtClient.readCmdBuff();
            } else {
                QMessageBox::warning(this, "Warning","Time out");
                return;
            }
        } else {
            return;
        }
    } else {
        return;
    }
}

void MainWindow::onMKD() {
    QInputDialog * inputDialog = new QInputDialog(this);
    inputDialog->setGeometry(inputDialog->parentWidget()->width()/2,inputDialog->parentWidget()->height()/2,400,200);
    inputDialog->setWindowTitle("提示");
    inputDialog->setLabelText("请输入文件夹名");
    QString DirName;
    if (inputDialog->exec() == QInputDialog::Accepted) {
        DirName = inputDialog->textValue();
        //qDebug()<<DirName;
    } else {
        return;
    }
    if (DirName.length()) {
        qtClient.sendCmd(MKD, DirName.toStdString().c_str());
        //wait for response
        if( qtClient.cmdSocket->waitForReadyRead(5000) == true){
            qtClient.readCmdBuff();
        } else {
            QMessageBox::warning(this, "Warning","Time out");
            return;
        }
    }
    refresh();
}

void MainWindow::onRename() {
    QList<QTableWidgetItem*> selected = fileTable->selectedItems();
    if (!selected.length()) {
        QMessageBox::information(this, "提示", "请选择一个文件");
        return;
    }
    if (selected[0]->text()=="..") {
        QMessageBox::information(this, "提示", "无法重命名");
        return;
    }
    QString fromName = selected[0]->text();

    QInputDialog * inputDialog = new QInputDialog(this);
    inputDialog->setGeometry(inputDialog->parentWidget()->width()/2,inputDialog->parentWidget()->height()/2,400,200);
    inputDialog->setWindowTitle("提示");
    inputDialog->setLabelText("请输入新名字");
    QString toName;
    if (inputDialog->exec() == QInputDialog::Accepted) {
        toName = inputDialog->textValue();
    } else {
        return;
    }

    qtClient.sendCmd(RNFR, fromName.toStdString().c_str());
    //wait for response
    if( qtClient.cmdSocket->waitForReadyRead(5000) == true){
        qtClient.readCmdBuff();
    } else {
        QMessageBox::warning(this, "Warning","Time out");
        return;
    }
    qtClient.sendCmd(RNTO, toName.toStdString().c_str());
    if( qtClient.cmdSocket->waitForReadyRead(5000) == true){
        qtClient.readCmdBuff();
    } else {
        QMessageBox::warning(this, "Warning","Time out");
        return;
    }
}

void MainWindow::append() {
    QList<QTableWidgetItem*> selected = fileTable->selectedItems();
    if (selected.length()<=2 || selected[1]->text() != "文件") {
        QMessageBox::information(this, "提示", "请选择一个文件");
        return;
    }
    QString localFileName = QFileDialog::getOpenFileName(this, "选择本地文件", "./");
    if(localFileName.length() == 0) {
        return;
    }
    QString remoteFileName = selected[0]->text();
    if (button_PASV->isChecked()) {
        qtClient.sendCmd(PASV,"");
        qtClient.sendCmd(APPE, remoteFileName.toStdString().c_str());
        qtClient.openPasvLink();
        if( qtClient.cmdSocket->waitForReadyRead(5000) == true){
            qtClient.readCmdBuff();
        } else {
            QMessageBox::warning(this, "Warning","Time out");
            return;
        }
    } else {
        PortOK = false;
        Port();
        if(!PortOK) {
            QMessageBox::information(this, "warning", "failed to build port connection");
            return;
        } else {
            qtClient.sendCmd(APPE, remoteFileName.toStdString().c_str());
            if( qtClient.cmdSocket->waitForReadyRead(5000) == true){
                qtClient.readCmdBuff();
            } else {
                QMessageBox::warning(this, "Warning","APPE Time out");
                return;
            }
            if (qtClient.dataSocket_PORT->waitForNewConnection(5000)) {
                 qtClient.dataSocket = (qtClient.dataSocket_PORT->nextPendingConnection());
            } else {
                QMessageBox::information(this, "Warning", qtClient.dataSocket_PORT->errorString());
                return;
            }
        }
    }

    int startPlace = selected[2]->text().toInt();
    QFile file(localFileName);
    file.open(QIODevice::ReadOnly);
    if (startPlace >= file.size()) {
        QMessageBox::warning(this, "Warning","服务器端文件起始位置大于本地文件大小");
        qtClient.dataSocket->close();
        return;
    }
    else {
        file.seek(startPlace);
    }

    while(!file.atEnd()) {
        QByteArray segement = file.read(8192);
        qtClient.dataSocket->write(segement);
        qtClient.dataSocket->flush();
    }
    qtClient.dataSocket->close();
    if( qtClient.cmdSocket->waitForReadyRead(5000) == true){
        qtClient.readCmdBuff();
    } else {
        QMessageBox::warning(this, "Warning","Time out");
        return;
    }
}

void MainWindow::rest() {

    QList<QTableWidgetItem*> selected = fileTable->selectedItems();
    if (selected.length()<=2 || selected[1]->text() != "文件") {
        QMessageBox::information(this, "提示", "请选择一个文件");
        return;
    }
    QString remoteFileName = selected[0]->text();

    QString localFileName = QFileDialog::getOpenFileName(this, "选择本地文件", "./");
    if(localFileName.length() == 0) {
        return;
    }
    QFile file(localFileName);
    file.open(QIODevice::ReadWrite);
    qint64 startPlace = file.size();
    file.seek(startPlace);
    //PASV MODE Download
    if(button_PASV->isChecked()) {
        qtClient.sendCmd(PASV,"");

        qtClient.sendCmd(REST, QString::number(startPlace).toStdString().c_str());
        waitNread();

        qtClient.sendCmd(RETR, remoteFileName.toStdString().c_str());
        qtClient.openPasvLink();
        //qtClient.dataSocket->moveToThread(m_thread);
        waitNread();

        while(qtClient.dataSocket->waitForReadyRead(5000)) {
            //qApp->processEvents();
            file.write(qtClient.dataSocket->readAll());
        }
        file.close();
        waitNread();
    }
    else if (button_PORT->isChecked()) {
        Port();
        qtClient.sendCmd(REST, QString::number(startPlace).toStdString().c_str());
        if( qtClient.cmdSocket->waitForReadyRead(5000) == true){
            qtClient.readCmdBuff();
        }
        qtClient.sendCmd(RETR, remoteFileName.toStdString().c_str());
        if (!qtClient.dataSocket_PORT) {
            QMessageBox::information(this, "Warning", "Invalid datasocketPort");
            return;
        }
        if (qtClient.dataSocket_PORT->waitForNewConnection(5000)) {
             qtClient.dataSocket = (qtClient.dataSocket_PORT->nextPendingConnection());
        } else {
            QMessageBox::information(this, "Warning", qtClient.dataSocket_PORT->errorString());
            return;
        }
        waitNread();
        while (qtClient.dataSocket->waitForReadyRead(5000)) {
            file.write(qtClient.dataSocket->readAll());
        }
        waitNread();
        file.close();
        qtClient.dataSocket->close();
    }
}

void MainWindow::deleteFile() {
    QList<QTableWidgetItem*> selected = fileTable->selectedItems();
    if (selected.length()<=2 || selected[1]->text() != "文件") {
        QMessageBox::information(this, "提示", "请选择一个文件");
        return;
    }
    QString remoteFileName = selected[0]->text();
    qtClient.sendCmd(DELE, remoteFileName.toStdString().c_str());
    waitNread();
    refresh();
}

void MainWindow::onRMD() {
    QList<QTableWidgetItem*> selected = fileTable->selectedItems();
    if (selected.length()<=2 || selected[1]->text() != "文件夹") {
        QMessageBox::information(this, "提示", "请选择一个文件夹");
        return;
    }
    QString remoteFileName = selected[0]->text();
    qtClient.sendCmd(RMD, remoteFileName.toStdString().c_str());
    waitNread();
    refresh();
}

void MainWindow::waitNread() {
    if( qtClient.cmdSocket->waitForReadyRead(3000) == true){
        qtClient.readCmdBuff();
    } else {
        QMessageBox::warning(this, "Warning","Time out");
        return;
    }
}
