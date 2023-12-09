#include "clientcore.h"

ClientCore::ClientCore() {} // 私有构造函数，确保单例

bool ClientCore::connectServer(QHostAddress address, quint16 port) {
    // 检查格式是否正确
    if(!address.isNull() && port > 0)
    {
        qDebug() << "正在连接到服务端...";
    }
    else
    {
        qDebug() << "IP地址或端口号格式错误！";
        return false;
    }

    serverAddress = address;
    serverPort = port;

    // 连接到服务端
    socket.connectToHost(serverAddress, serverPort);

    // 检查连接是否成功
    if(socket.waitForConnected(3000))
    {
        qDebug() << "连接成功！";

        // 发送数据到服务端
        QString message = "你好，服务端！";
        socket.write(message.toUtf8());

        // 等待数据接收
        if(socket.waitForReadyRead(3000))
        {
            // 读取服务端的响应
            QString response = QString::fromUtf8(socket.readAll());
            qDebug() << "收到服务端消息：" << response;
        }
        else
        {
            qDebug() << "未能接收到服务端响应";
        }

        // 关闭连接
        //core->socket.disconnectFromHost();
    }
    else
    {
        qDebug() << "连接失败：" << socket.errorString();
    }
}

bool ClientCore::registerRequest(QString userName, QString password) {
    // 定义正则表达式模式，表示只包含数字、英文字母和特殊字符
    QRegularExpression regex("^[a-zA-Z0-9!@#$%^&*()-_+=<>?]+$");
    // 使用正则表达式匹配字符串
    if (regex.match(userName).hasMatch() && regex.match(password).hasMatch()) {
        // 发送注册请求到服务端
        QString message = QString("register;%1;%2").arg(userName).arg(password);
        socket.write(message.toUtf8());

        // 等待数据接收
        if (socket.waitForReadyRead(3000)) {
            // 读取服务端的响应
            QString response = QString::fromUtf8(socket.readAll());
            qDebug() << "收到服务端消息：" << response;
        } else qDebug() << "未能接收到服务端响应";
    }
}
