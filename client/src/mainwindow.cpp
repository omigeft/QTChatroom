#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 获取核心实例
    core = &ClientCore::getInstance();
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
    // 根据IP和端口进行连接
    if (core->connectServer(QHostAddress(ui->IPInput->text()), ui->PortInput->text().toInt())) {
        //进入登录界面
        LoginWindow * loginwindow = new LoginWindow();
        this->close();
        loginwindow->show();
    }
}
