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

        // 测试发送数据

        // 创建一个 JSON 对象
        QJsonObject jsonObj;
        jsonObj["type"] = "test";
        jsonObj["state"] = "request";

        // 创建一个嵌套的JSON数据对象
        QJsonObject dataObj;
        dataObj["test"] = "test";

        // 将嵌套的JSON数据对象添加到 "data" 字段
        jsonObj["data"] = dataObj;

        // 使用 QJsonDocument 生成 JSON 字符串
        QJsonDocument doc(jsonObj);
        QString message = doc.toJson(QJsonDocument::Compact);

        // 发送数据到服务端
        socket.write(message.toUtf8());

        // 等待数据接收
        if(socket.waitForReadyRead(3000))
        {
            // 读取服务端的响应
            QString response = QString::fromUtf8(socket.readAll());
            qDebug() << "收到服务端消息：" << response;

            if(!checkResponseMessage(response, jsonObj["type"].toString()))
            {
                return false;
            }

            qDebug() << "收到响应：测试成功";
        }
        else
        {
            qDebug() << "未能接收到服务端响应";
            return false;
        }

        // 关闭连接
        //core->socket.disconnectFromHost();
    }
    else
    {
        qDebug() << "连接失败：" << socket.errorString();
        return false;
    }

    return true;
}

bool ClientCore::registerRequest(QString userName, QString password) {
    // 检查用户名和密码是否在6-20个字符之间
    if (userName.length() < 6 || userName.length() > 20) {
        qDebug() << "用户名长度不符合要求!";
        return false;
    }
    if (password.length() < 6 || password.length() > 20) {
        qDebug() << "密码长度不符合要求!";
        return false;
    }

    // 定义正则表达式模式，表示只包含数字、英文字母和特殊字符
    QRegularExpression regex("^[a-zA-Z0-9!@#$%^&*()-_+=<>?]+$");
    // 使用正则表达式匹配字符串
    if (regex.match(userName).hasMatch() && regex.match(password).hasMatch()) {
        QJsonObject jsonObj = baseJsonObj("register", "request");

        // 编辑数据字段
        QJsonObject dataObj = jsonObj["data"].toObject();
        dataObj["userName"] = userName;
        dataObj["password"] = password;
        jsonObj["data"] = dataObj;

        sendJsonObj(jsonObj);

        // 等待数据接收
        if (socket.waitForReadyRead(3000)) {
            // 读取服务端的响应
            QString response = QString::fromUtf8(socket.readAll());
            qDebug() << "收到服务端消息：" << response;

            if (!checkResponseMessage(response, jsonObj["type"].toString())) {
                return false;
            }

            QJsonDocument resJsonDoc = QJsonDocument::fromJson(response.toUtf8());
            QJsonObject resJsonObj = resJsonDoc.object();
            QJsonObject resDataObj = resJsonObj["data"].toObject();

            if (resDataObj["userName"].toString() != userName) {
                qDebug() << "收到响应：用户名不匹配";
                return false;
            }

            qDebug() << "收到响应：新账号注册成功";

        } else {
            qDebug() << "未能接收到服务端响应";
            return false;
        }
    } else {
        qDebug() << "用户名和密码包含不允许的字符！";
        return false;
    }

    return true;
}

bool ClientCore::loginRequest(QString userName, QString password) {
    // 检查用户名和密码是否在6-20个字符之间
    if (userName.length() < 6 || userName.length() > 20) {
        qDebug() << "用户名长度不符合要求!";
        return false;
    }
    if (password.length() < 6 || password.length() > 20) {
        qDebug() << "密码长度不符合要求!";
        return false;
    }

    // 定义正则表达式模式，表示只包含数字、英文字母和特殊字符
    QRegularExpression regex("^[a-zA-Z0-9!@#$%^&*()-_+=<>?]+$");
    // 使用正则表达式匹配字符串
    if (regex.match(userName).hasMatch() && regex.match(password).hasMatch()) {
        QJsonObject jsonObj = baseJsonObj("login", "request");

        // 编辑数据字段
        QJsonObject dataObj = jsonObj["data"].toObject();
        dataObj["userName"] = userName;
        dataObj["password"] = password;
        jsonObj["data"] = dataObj;

        sendJsonObj(jsonObj);

        // 等待数据接收
        if (socket.waitForReadyRead(3000)) {
            // 读取服务端的响应
            QString response = QString::fromUtf8(socket.readAll());
            qDebug() << "收到服务端消息：" << response;

            if (!checkResponseMessage(response, jsonObj["type"].toString())) {
                return false;
            }

            QJsonDocument resJsonDoc = QJsonDocument::fromJson(response.toUtf8());
            QJsonObject resJsonObj = resJsonDoc.object();
            QJsonObject resDataObj = resJsonObj["data"].toObject();

            if (resDataObj["userName"].toString() != userName) {
                qDebug() << "收到响应：用户名不匹配";
                return false;
            }

            qDebug() << "收到响应：登录成功";

            currentUserName = userName;

        } else {
            qDebug() << "未能接收到服务端响应";
            return false;
        }
    } else {
        qDebug() << "用户名和密码包含不允许的字符！";
        return false;
    }

    return true;
}

bool ClientCore::checkResponseMessage(QString message, QString type) {
    // 解析服务端响应的 JSON 字符串
    QJsonDocument resJsonDoc = QJsonDocument::fromJson(message.toUtf8());

    if (resJsonDoc.isNull() || !resJsonDoc.isObject()) {
        qDebug() << "数据报文解析失败!";
        return false;
    }

    QJsonObject resJsonObj = resJsonDoc.object();

    if (resJsonObj["type"].toString() != type) {
        qDebug() << "响应数据报文类型不正确!";
        return false;
    }

    if (resJsonObj["state"].toString() != "success") {
        qDebug() << "收到响应：请求没有成功";
        return false;
    }

    return true;
}

QJsonObject ClientCore::baseJsonObj(const QString &type, const QString &state) {
    // 创建一个 JSON 对象
    QJsonObject jsonObj;
    jsonObj["type"] = type;
    jsonObj["state"] = state;

    // 创建一个嵌套的JSON数据对象
    QJsonObject dataObj;
    jsonObj["data"] = dataObj;

    return jsonObj;
}

void ClientCore::sendJsonObj(const QJsonObject &jsonObj) {
    // 使用 QJsonDocument 生成 JSON 字符串，并发送报文
    QJsonDocument jsonDoc(jsonObj);
    QString message = jsonDoc.toJson(QJsonDocument::Compact);
    socket.write(message.toUtf8());
}
