#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwindow.h"
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_CloseButton_clicked()
{
    this->close();
}

void MainWindow::on_ConnectButton_clicked()
{
    //根据IP和端口进行连接
    //并确定连接结果
    bool ConnectFlag = ui->IPInput->text()=="1";

    if(ConnectFlag)
    {
        //检查正确后进入登录界面
        LoginWindow * loginwindow = new LoginWindow();
        this->close();
        loginwindow->show();

    }
    else
    {
        QMessageBox::information(this,"输入有误","请输入正确的IP地址或端口号");
    }
}
