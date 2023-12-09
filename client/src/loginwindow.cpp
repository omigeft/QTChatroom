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


    /*
    bool isRegistered;//判断是否注册
    bool isPassWordEmpty;//判断密码是否为空
    isRegistered = ui->NameInput->text()=="1";
    isPassWordEmpty=ui->PassWordInput->text()=="";
    if(isRegistered)//已经被注册
    {
        QMessageBox::information(this,"注册有误","该用户名已被注册");
    }
    else
    {
        if(!isPassWordEmpty)
            QMessageBox::information(this,"成功","注册成功");
        else
            QMessageBox::information(this,"注册有误","密码不能为空");
    }
    */
}

void LoginWindow::on_LoginButton_clicked()
{
    // 发送登录请求到服务端
    QString message = "我要登录";
    core->socket.write(message.toUtf8());

    // 等待数据接收
    if(core->socket.waitForReadyRead(3000))
    {
        // 读取服务端的响应
        QString response = QString::fromUtf8(core->socket.readAll());
        qDebug() << "收到服务端消息：" << response;
    }
    else
    {
        qDebug() << "未能接收到服务端响应";
    }

    /*
    //判断用户是否存在
    bool isExist;
    isExist = ui->NameInput->text()=="1";
    //用户不存在
    if(!isExist){
        QMessageBox::information(this,"登录有误","该用户不存在\n请注册账户");
    }
    else {
        //切换到用户列表
        ChatList * userChatList = new ChatList();
        userChatList->UserName = ui->NameInput->text();
        this->close();
        userChatList->initChatList();
        userChatList->show();
    }
    */
}
