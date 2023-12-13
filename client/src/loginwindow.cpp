#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "mainwindow.h"
#include "chatlist.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    // 获取核心实例
    core = &ClientCore::getInstance();
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_CloseButton_clicked()
{
    this->close();
}

void LoginWindow::on_RegisterButton_clicked()
{
    core->registerRequest(ui->NameInput->text(), ui->PassWordInput->text());
}

void LoginWindow::on_LoginButton_clicked()
{
    if (core->loginRequest(ui->NameInput->text(), ui->PassWordInput->text())) {
        //切换到用户列表
        qDebug() << "登录成功1";
        ChatList * userChatList = new ChatList();
        qDebug() << "登录成功2";
        this->close();
        qDebug() << "登录成功3";
        userChatList->initChatList();
        qDebug() << "登录成功4";
        userChatList->show();
    }
}
