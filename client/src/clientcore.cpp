#include "chat.h"
#include "clientcore.h"

bool ClientCore::connectServer(const QHostAddress &address, const quint16 &port) {
    // 检查格式是否正确
    if(!address.isNull() && port > 0) {
        qDebug() << "正在连接到服务端...";
    } else {
        qDebug() << "IP地址或端口号格式错误！";
        return false;
    }

    serverAddress = address;
    serverPort = port;

    // 连接到服务端
    socket.connectToHost(serverAddress, serverPort);

    // 检查连接是否成功
    if(socket.waitForConnected(3000)) {
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

        QString response;

        if(sendAndWait(response, jsonObj)) {
            qDebug() << "connectServer()收到服务端消息：" << response;

            if(!checkMessage(response, jsonObj["type"].toString(), "success")) {
                return false;
            }

            qDebug() << "收到响应：测试成功";
        } else {
            qDebug() << "未能接收到服务端响应";
            return false;
        }
    } else {
        qDebug() << "连接失败：" << socket.errorString();
        return false;
    }

    return true;
}

bool ClientCore::registerRequest(const QString &userName, const QString &password) {
    // 检查用户名和密码是否在1-20个字符之间
    if (userName.length() < 1 || userName.length() > 20) {
        qDebug() << "用户名长度不符合要求!";
        return false;
    }
    if (password.length() < 6 || password.length() > 20) {
        qDebug() << "密码长度不符合要求!";
        return false;
    }

    // 定义正则表达式模式，表示只包含数字、英文字母和特殊字符
    QRegularExpression regex("^[a-zA-Z0-9!@#$%^&*()-_+=<>?]+$");
    // 使用正则表达式匹配检查密码字符串
    if (regex.match(password).hasMatch()) {
        QJsonObject jsonObj = baseJsonObj("register", "request");

        // 编辑数据字段
        QJsonObject dataObj = jsonObj["data"].toObject();
        dataObj["userName"] = userName;
        dataObj["password"] = password;
        jsonObj["data"] = dataObj;

        QString response;

        // 等待数据接收
        if (sendAndWait(response, jsonObj)) {
            if (!checkMessage(response, jsonObj["type"].toString(), "success")) {
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

bool ClientCore::loginRequest(const QString &userName, const QString &password) {
    // 检查用户名和密码是否在1-20个字符之间
    if (userName.length() < 1 || userName.length() > 20) {
        qDebug() << "用户名长度不符合要求!";
        return false;
    }
    if (password.length() < 6 || password.length() > 20) {
        qDebug() << "密码长度不符合要求!";
        return false;
    }

    // 定义正则表达式模式，表示只包含数字、英文字母和特殊字符
    QRegularExpression regex("^[a-zA-Z0-9!@#$%^&*()-_+=<>?]+$");
    // 使用正则表达式匹配检查密码字符串
    if (regex.match(password).hasMatch()) {
        QJsonObject jsonObj = baseJsonObj("login", "request");

        // 编辑数据字段
        QJsonObject dataObj = jsonObj["data"].toObject();
        dataObj["userName"] = userName;
        dataObj["password"] = password;
        jsonObj["data"] = dataObj;

        QString response;

        // 等待数据接收
        if (sendAndWait(response, jsonObj)) {
            if (!checkMessage(response, jsonObj["type"].toString(), "success")) {
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

bool ClientCore::createChatroomRequest(const QString &chatName, const QString &creatorName) {
    // 检查聊天室名称是否在1-20个字符之间
    if (chatName.length() < 1 || chatName.length() > 20) {
        qDebug() << "聊天室名称长度不符合要求!";
        return false;
    }

    QJsonObject jsonObj = baseJsonObj("createChatroom", "request");

    // 编辑数据字段
    QJsonObject dataObj = jsonObj["data"].toObject();
    dataObj["chatName"] = chatName;
    dataObj["creatorName"] = creatorName;
    jsonObj["data"] = dataObj;

    QString response;

    // 等待数据接收
    if (sendAndWait(response, jsonObj)) {
        if (!checkMessage(response, jsonObj["type"].toString(), "success")) {
            return false;
        }

        QJsonDocument resJsonDoc = QJsonDocument::fromJson(response.toUtf8());
        QJsonObject resJsonObj = resJsonDoc.object();
        QJsonObject resDataObj = resJsonObj["data"].toObject();

        if (resDataObj["chatName"].toString() != chatName) {
            qDebug() << "收到响应：聊天室名称不匹配";
            return false;
        }

        qDebug() << "收到响应：创建聊天室成功";

    } else {
        qDebug() << "未能接收到服务端响应";
        return false;
    }

    return true;
}

bool ClientCore::getChatListRequest(const QString &userName) {
    QJsonObject jsonObj = baseJsonObj("getChatList", "request");

    // 编辑数据字段
    QJsonObject dataObj = jsonObj["data"].toObject();
    dataObj["userName"] = userName;
    jsonObj["data"] = dataObj;

    QString response;

    // 等待数据接收
    if (sendAndWait(response, jsonObj)) {
        if (!checkMessage(response, jsonObj["type"].toString(), "success")) {
            return false;
        }

        QJsonDocument resJsonDoc = QJsonDocument::fromJson(response.toUtf8());
        QJsonObject resJsonObj = resJsonDoc.object();
        QJsonObject resDataObj = resJsonObj["data"].toObject();

        if (resDataObj["userName"].toString() != userName) {
            qDebug() << "收到响应：用户名不匹配";
            return false;
        }

        // 获取聊天室列表
        QJsonArray joinedChatList = resDataObj["joinedChatList"].toArray();
        joinedList.clear();
        for (int i = 0; i < joinedChatList.size(); ++i) {
            joinedList.append(joinedChatList[i].toString());
        }

        QJsonArray unjoinedChatList = resDataObj["unjoinedChatList"].toArray();
        unjoinedList.clear();
        for (int i = 0; i < unjoinedChatList.size(); ++i) {
            unjoinedList.append(unjoinedChatList[i].toString());
        }

        qDebug() << "收到响应：获取用户可访问的聊天室列表成功";

    } else {
        qDebug() << "未能接收到服务端响应";
        return false;
    }

    return true;
}

QJsonArray ClientCore::getChatUserListRequest(const QString &chatName) {
    QJsonObject jsonObj = baseJsonObj("getChatUserList", "request");

    // 编辑数据字段
    QJsonObject dataObj = jsonObj["data"].toObject();
    dataObj["chatName"] = chatName;
    jsonObj["data"] = dataObj;

    QString response;

    // 等待数据接收
    if (sendAndWait(response, jsonObj)) {
        if (!checkMessage(response, jsonObj["type"].toString(), "success")) {
            return QJsonArray();
        }

        QJsonDocument resJsonDoc = QJsonDocument::fromJson(response.toUtf8());
        QJsonObject resJsonObj = resJsonDoc.object();
        QJsonObject resDataObj = resJsonObj["data"].toObject();

        if (resDataObj["chatName"].toString() != chatName) {
            qDebug() << "收到响应：聊天室名称不匹配";
            return QJsonArray();
        }

        // 获取聊天室用户列表
        QJsonArray chatUserList = resDataObj["chatUserList"].toArray();

        qDebug() << "收到响应：获取聊天室用户列表成功";

        return chatUserList;

    } else {
        qDebug() << "未能接收到服务端响应";
        return QJsonArray();
    }
}

QJsonArray ClientCore::getMessageRequest(const QString &chatName, const int latestMessageID) {
    QJsonObject jsonObj = baseJsonObj("getMessage", "request");

    // 编辑数据字段
    QJsonObject dataObj = jsonObj["data"].toObject();
    dataObj["chatName"] = chatName;
    dataObj["latestMessageID"] = latestMessageID;
    jsonObj["data"] = dataObj;

    QString response;

    // 等待数据接收
    if (sendAndWait(response, jsonObj)) {
        if (!checkMessage(response, jsonObj["type"].toString(), "success")) {
            return QJsonArray();
        }

        QJsonDocument resJsonDoc = QJsonDocument::fromJson(response.toUtf8());
        QJsonObject resJsonObj = resJsonDoc.object();
        QJsonObject resDataObj = resJsonObj["data"].toObject();

        if (resDataObj["chatName"].toString() != chatName) {
            qDebug() << "收到响应：聊天室名称不匹配";
            return QJsonArray();
        }

        // 获取聊天室消息列表
        QJsonArray messageList = resDataObj["messageList"].toArray();

        qDebug() << "收到响应：获取聊天室消息列表成功";

        return messageList;

    } else {
        qDebug() << "未能接收到服务端响应";
        return QJsonArray();
    }
}

bool ClientCore::sendMessageRequest(const QString &chatName, const QString &senderName, const QString &message) {
    QJsonObject jsonObj = baseJsonObj("sendMessage", "request");

    // 编辑数据字段
    QJsonObject dataObj = jsonObj["data"].toObject();
    dataObj["chatName"] = chatName;
    dataObj["senderName"] = senderName;
    dataObj["message"] = message;
    jsonObj["data"] = dataObj;

    QString response;

    // 等待数据接收
    if (sendAndWait(response, jsonObj)) {
        if (!checkMessage(response, jsonObj["type"].toString(), "success")) {
            return false;
        }

        QJsonDocument resJsonDoc = QJsonDocument::fromJson(response.toUtf8());
        QJsonObject resJsonObj = resJsonDoc.object();
        QJsonObject resDataObj = resJsonObj["data"].toObject();

        if (resDataObj["chatName"].toString() != chatName) {
            qDebug() << "收到响应：聊天室名称不匹配";
            return false;
        }

        qDebug() << "收到响应：发送消息成功";

        return true;

    } else {
        qDebug() << "未能接收到服务端响应";
        return false;
    }
}

void ClientCore::onReadyRead() {
    // 读取服务端的消息
    QString message = QString::fromUtf8(socket.readAll());

    qDebug() << "onReadyRead()收到服务端消息：" << message;

    emit readMessage(message);

    // 无聊天室打开时，不处理消息
    if (nameChatMap.isEmpty()) {
        return;
    }

    if (!checkMessage(message, "synchronization", "remind")) {
        return;
    }

    QJsonDocument resJsonDoc = QJsonDocument::fromJson(message .toUtf8());
    QJsonObject resJsonObj = resJsonDoc.object();
    QJsonObject resDataObj = resJsonObj["data"].toObject();

    // 获取聊天室名称
    QString chatName = resDataObj["chatName"].toString();

    // 被提醒，表示有新消息，需要更新消息列表（使用映射QMap<QString, Chat*> nameChatMap）
    nameChatMap[chatName]->refreshChat();
}

ClientCore::ClientCore() {
    // 收到服务器消息的信号槽连接
    connect(&socket, &QTcpSocket::readyRead, this, &ClientCore::onReadyRead);
}

bool ClientCore::checkMessage(const QString &message, const QString &type, const QString &state) {
    // 解析服务端响应的 JSON 字符串
    QJsonDocument resJsonDoc = QJsonDocument::fromJson(message.toUtf8());

    if (resJsonDoc.isNull() || !resJsonDoc.isObject()) {
        qDebug() << "数据报文解析失败!";
        return false;
    }

    QJsonObject resJsonObj = resJsonDoc.object();

    if (resJsonObj["type"].toString() != type) {
        qDebug() << "响应数据报文类型不正确!" << resJsonObj["type"].toString() << "!=" << type;
        return false;
    }

    if (resJsonObj["state"].toString() != state) {
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

bool ClientCore::sendAndWait(QString &response, const QJsonObject &jsonObj) {
//    qDebug() << "dbg0  " << response;
    QTimer timer;
    QEventLoop loop;
    bool readFlag = false;  // 区分是否在规定时间内接收到数据的标志

    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    connect(this, &ClientCore::readMessage, &loop, [&](const QString &message){
        response = message;
        readFlag = true;
        loop.quit();
    });

    timer.start(3000);
    sendJsonObj(jsonObj);
    loop.exec();

    // 判断是否在规定时间内接收到数据
    return readFlag;
}
