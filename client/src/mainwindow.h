#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPushButton>
#include <QDesktopWidget>
#include <QTextEdit>
#include <string>
#include <QStringList>
#include <QTableWidget>
#include <QRegExp>
#include <QFileDialog>
#include <QHostInfo>
#include <QMessageBox>
#include <QtGlobal>
#include <QTime>
#include <QInputDialog>
#include <QMouseEvent>
#include <QThread>
#include "qtclient.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void connectToServer();
    void logIn();
    void quit();
    void askForSYST();
    void askForPWD();
    void typeBinary();
    void refresh();
    void download();
    void upload();
    void append();
    void rest();
    void deleteFile();
    void getLocalIP();
    void Port();
    void onMKD();
    void onRMD();
    void onRename();
    void waitNread();
    unsigned short generateRandPort();
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QtClient qtClient;
    QString localIP;
    QLabel* label_host, *label_username, *label_password, *label_port, *label_mode;
    QLineEdit *line_host, *line_username, *line_password, *line_port, *line_curpath;
    QTextEdit* ioWidget;
    QRadioButton* button_PORT, *button_PASV;
    QPushButton* push_connect, *push_quit, *push_login, *push_fresh, *push_mkdir, *push_download, *push_upload,*push_rename;
    QPushButton* push_append, *push_rest, *push_delete,*push_rmd;
    QButtonGroup* buttonGroup_mode;
    QWidget* centralWidget;
    QGridLayout* mainLayout;
    QHBoxLayout* topLayout,*second_topLayout, *second_secondLayout, *second_thirdLayout;
    QVBoxLayout *secondLayout;
    QTableWidget* fileTable;
    unsigned short Port_port;
    bool PortOK;
public slots:
    void handleRetBuff(cmdType type);
    void onTableItemClicked(int row, int column);
    void onSocketDisconnect();
};
#endif // MAINWINDOW_H
