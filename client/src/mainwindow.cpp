#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 获取公共数据实例
    pd = &PublicData::getInstance();
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

    // 服务端地址
    pd->serverAddress = ui->IPInput->text();
    // 服务端端口
    pd->serverPort = ui->PortInput->text().toInt();

    // 检查格式是否正确
    if(!QHostAddress(pd->serverAddress).isNull() && pd->serverPort > 0)
    {
        qDebug() << "正在连接到服务端...";
    }
    else
    {
        qDebug() << "IP地址或端口号格式错误！";
        return;
    }

    // 连接到服务端
    pd->socket.connectToHost(pd->serverAddress, pd->serverPort);

    // 检查连接是否成功
    if(pd->socket.waitForConnected(3000))
    {
        qDebug() << "连接成功！";

        // 发送数据到服务端
        QString message = "你好，服务端！";
        pd->socket.write(message.toUtf8());

        // 等待数据接收
        if(pd->socket.waitForReadyRead(3000))
        {
            // 读取服务端的响应
            QString response = QString::fromUtf8(pd->socket.readAll());
            qDebug() << "收到服务端消息：" << response;
        }
        else
        {
            qDebug() << "未能接收到服务端响应";
        }

        // 关闭连接
        //pd->socket.disconnectFromHost();

        //进入登录界面
        LoginWindow * loginwindow = new LoginWindow();
        this->close();
        loginwindow->show();
    }
    else
    {
        qDebug() << "连接失败：" << pd->socket.errorString();
    }
}
