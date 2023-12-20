#include "servercore.h"

bool ServerCore::createServer(QHostAddress address, quint16 port) {
    // 检查格式是否正确
    if(!address.isNull() && port > 0) {
        qDebug() << "正在创建服务端...";
    } else {
        qDebug() << "IP地址或端口号格式错误！";
        return false;
    }
    if (!server.listen(address, port)) {
        qDebug() << "Server listen failed!";
        return false;
    }
    serverAddress = address;
    serverPort = port;
    qDebug() << "Server listen on " << serverAddress.toString() << ":" << serverPort;

    if (!createDatabase()) {
        qDebug() << "Database connection failed!";
        return false;
    }

    return true;
}

bool ServerCore::createDatabase() {
    // 创建数据库（设置引擎）
    db = QSqlDatabase::addDatabase("QSQLITE");
    // 获取当前工作目录
    QString currentDir = QDir::currentPath();
    // 构建数据库文件的路径，这里假设你要保存的数据库文件名为mydatabase.db
    QString databasePath = currentDir + QDir::separator() + "server_db.db";
    qDebug() << "Database saved in " << databasePath;
    // 设置数据库保存路径
    db.setDatabaseName(databasePath);
    if (!db.open()) return false;
    qDebug() << "Database connection successful!!";

    QSqlQuery query;
    // 创建用户表user
    query.exec(
        "CREATE TABLE IF NOT EXISTS user ("
        "u_id    INT PRIMARY KEY,"              // user id
        "u_name  VARCHAR(20) UNIQUE NOT NULL,"  // user name
        "pw      VARCHAR(20) NOT NULL,"         // password
        "su_t    DATETIME,"                     // sign up time
        "sd_t    DATETIME);"                    // sign down time
        );
    if (query.lastError().isValid()) {
        qDebug() << query.lastError();
        return false;
    }

    // 创建聊天室表chatroom
    query.exec(
        "CREATE TABLE IF NOT EXISTS chatroom ("
        "c_id    INT PRIMARY KEY,"              // chatroom id
        "c_name  VARCHAR(20) UNIQUE NOT NULL,"  // chatroom name
        "c_u_id  INT NOT NULL,"                 // creator user id
        "cr_t    DATETIME,"                     // creation time
        "ds_t    DATETIME);"                    // dissolution time
        );
    if (query.lastError().isValid()) {
        qDebug() << query.lastError();
        return false;
    }

    // 创建用户-聊天室关系表user_chatroom
    query.exec(
        "CREATE TABLE IF NOT EXISTS user_chatroom ("
        "u_id    INT NOT NULL,"                 // user id
        "c_id    INT NOT NULL,"                 // chatroom id
        "j_t     DATETIME,"                     // join time
        "q_t     DATETIME,"                     // quit time
        "PRIMARY KEY(u_id, c_id),"
        "FOREIGN KEY(u_id) REFERENCES user(u_id),"
        "FOREIGN KEY(c_id) REFERENCES chatroom(c_id));"
        );
    if (query.lastError().isValid()) {
        qDebug() << query.lastError();
        return false;
    }

    // 创建消息表message
    query.exec(
        "CREATE TABLE IF NOT EXISTS message ("
        "m_id    INT PRIMARY KEY,"              // message id
        "m_u_id  INT NOT NULL,"                 // user id
        "m_c_id  INT NOT NULL,"                 // chatroom id
        "m_t     DATETIME,"                     // message time
        "m_text  TEXT,"                         // message text
        "FOREIGN KEY(m_u_id) REFERENCES user(u_id),"
        "FOREIGN KEY(m_c_id) REFERENCES chatroom(c_id));"
        );
    if (query.lastError().isValid()) {
        qDebug() << query.lastError();
        return false;
    }

    query.exec("SELECT MAX(u_id) FROM user;");
    if (query.lastError().isValid()) {
        qDebug() << query.lastError();
        return false;
    }
    if (query.next())
        maxUserNumber = query.value(0).toInt(); // 获取用户号最大值

    query.exec("SELECT MAX(c_id) FROM chatroom;");
    if (query.lastError().isValid()) {
        qDebug() << query.lastError();
        return false;
    }
    if (query.next())
        maxChatroomNumber = query.value(0).toInt(); // 获取聊天室号最大值

    query.exec("SELECT MAX(m_id) FROM message;");
    if (query.lastError().isValid()) {
        qDebug() << query.lastError();
        return false;
    }
    if (query.next())
        maxMessageNumber = query.value(0).toInt(); // 获取消息号最大值

    userTableModel = new QSqlTableModel;
    userTableModel->setTable("user"); // 替换为你的表名
    //userTableModel->setEditStrategy(QSqlTableModel::OnFieldChange);    // 在界面上修改后数据立刻保存到数据库
    userTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);   // 将将编辑数据库中值的策略设置为[在调用 submitAll() 或 revertAll() 之前，所有更改都将缓存在模型中（即在界面上修改数据后不会立刻存入数据库）]
    userTableModel->select(); // 执行查询以加载数据
    userTableModel->setHeaderData(0, Qt::Horizontal, "用户ID");
    userTableModel->setHeaderData(1, Qt::Horizontal, "用户名");
    userTableModel->setHeaderData(2, Qt::Horizontal, "密码");
    userTableModel->setHeaderData(3, Qt::Horizontal, "注册时间");
    userTableModel->setHeaderData(4, Qt::Horizontal, "注销时间");

    chatTableModel = new QSqlTableModel;
    chatTableModel->setTable("chatroom"); // 替换为你的表名
    //chatTableModel->setEditStrategy(QSqlTableModel::OnFieldChange);    // 在界面上修改后数据立刻保存到数据库
    chatTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);   // 将将编辑数据库中值的策略设置为[在调用 submitAll() 或 revertAll() 之前，所有更改都将缓存在模型中（即在界面上修改数据后不会立刻存入数据库）]
    chatTableModel->select(); // 执行查询以加载数据
    chatTableModel->setHeaderData(0, Qt::Horizontal, "聊天室ID");
    chatTableModel->setHeaderData(1, Qt::Horizontal, "聊天室名");
    chatTableModel->setHeaderData(2, Qt::Horizontal, "创建者用户ID");
    chatTableModel->setHeaderData(3, Qt::Horizontal, "创建时间");
    chatTableModel->setHeaderData(4, Qt::Horizontal, "解散时间");

    return true;
}

bool ServerCore::registerAccount(const QString &userName, const QString &password) {
    QSqlQuery query;

    // 检查用户名是否已存在
    query.exec(QString("SELECT u_name FROM user WHERE u_name = '%1';").arg(userName));
    if (query.lastError().isValid()) {
        qDebug() << query.lastError();
        return false;
    }
    if (query.next()) {
        qDebug() << "用户名已存在!";
        return false;
    }

    // 插入新用户
    QString sql_statement =
            "INSERT INTO user (u_id, u_name, pw, su_t, sd_t) VALUES " +
            QString("(%1,'%2','%3','%4',%5);")
            .arg(++maxUserNumber)
            .arg(userName)
            .arg(password)
            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
            .arg("NULL");

    // 执行SQL语句
    query.exec(sql_statement);
    if (query.lastError().isValid()) {
        qDebug() << query.lastError();
        return false;
    }

    // 更新界面显示的用户表
    userTableModel->select();

    return true;
}

bool ServerCore::loginAccount(QTcpSocket *socket, const QString &userName, const QString &password) {
    QSqlQuery query;

    // 检查用户名是否已存在
    query.exec(QString("SELECT u_name FROM user WHERE u_name = '%1';").arg(userName));
    if (query.lastError().isValid()) {
        qDebug() << query.lastError();
        return false;
    }
    query.next();
    if (query.value(0).toString().isEmpty()) {
        qDebug() << "用户名不存在!";
        return false;
    }

    // 检查密码是否正确
    query.exec(QString("SELECT pw FROM user WHERE u_name = '%1';").arg(userName));
    if (query.lastError().isValid()) {
        qDebug() << query.lastError();
        return false;
    }
    query.next();
    if (query.value(0).toString() != password) {
        qDebug() << "密码错误!";
        return false;
    }

    // 该账号与该连接绑定
    userSocketMap.insert(userName, socket);

    return true;
}

bool ServerCore::createChatroom(const QString &chatroomName, const QString &userName) {
    QSqlQuery query;

    // 检查聊天室名是否已存在
    query.exec(QString("SELECT c_id FROM chatroom WHERE c_name = '%1';").arg(chatroomName));
    if (query.lastError().isValid()) {
        qDebug() << query.lastError();
        return false;
    }
    if (query.next()) {
        qDebug() << "聊天室名已存在!";
        return false;
    }

    // 检查用户名是否已存在
    query.exec(QString("SELECT u_id FROM user WHERE u_name = '%1';").arg(userName));
    if (query.lastError().isValid()) {
        qDebug() << query.lastError();
        return false;
    }
    if (!query.next()) {
        qDebug() << "用户名不存在!";
        return false;
    }

    int userID = query.value(0).toInt();

    // 插入新聊天室和新用户-聊天室关系，以事务的方式执行，保证原子性
    query.exec("BEGIN;");
    query.exec(QString(
        "INSERT INTO chatroom (c_id, c_name, c_u_id, cr_t, ds_t) VALUES "
        "(%1,'%2',%3,'%4',%5);")
        .arg(++maxChatroomNumber)
        .arg(chatroomName)
        .arg(userID)
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
        .arg("NULL"));
    query.exec(QString(
        "INSERT INTO user_chatroom (u_id, c_id, j_t, q_t) VALUES "
        "(%1,%2,'%3',%4);")
        .arg(userID)
        .arg(maxChatroomNumber)
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
        .arg("NULL"));
    query.exec("COMMIT;");
    if (query.lastError().isValid()) {
        qDebug() << query.lastError();
        return false;
    }

    // 更新界面显示的聊天室表
    chatTableModel->select();

    return true;
}

bool ServerCore::joinChatroom(const QString &chatroomName, const QString &userName) {
    QSqlQuery query;

    // 检查聊天室名是否已存在
    query.exec(QString("SELECT c_id FROM chatroom WHERE c_name = '%1';").arg(chatroomName));
    if (query.lastError().isValid()) {
        qDebug() << query.lastError();
        return false;
    }
    if (!query.next()) {
        qDebug() << "聊天室名不存在!";
        return false;
    }

    int chatroomID = query.value(0).toInt();

    // 检查用户名是否已存在
    query.exec(QString("SELECT u_id FROM user WHERE u_name = '%1';").arg(userName));
    if (query.lastError().isValid()) {
        qDebug() << query.lastError();
        return false;
    }
    if (!query.next()) {
        qDebug() << "用户名不存在!";
        return false;
    }

    int userID = query.value(0).toInt();

    // 插入新用户-聊天室关系
    query.exec(QString(
        "INSERT INTO user_chatroom (u_id, c_id, j_t, q_t) VALUES "
        "(%1,%2,'%3',%4);")
        .arg(userID)
        .arg(chatroomID)
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
        .arg("NULL"));
    if (query.lastError().isValid()) {
        qDebug() << query.lastError();
        return false;
    }

    return true;
}

QJsonArray ServerCore::getJoinedChatList(const QString &userName) {
    QJsonArray chatList;
    QSqlQuery query;

    // 查询该用户已加入的所有聊天室
    query.exec(QString(
        "SELECT c_name FROM user_chatroom "
        "INNER JOIN chatroom ON user_chatroom.c_id = chatroom.c_id "
        "INNER JOIN user ON user_chatroom.u_id = user.u_id "
        "WHERE u_name = '%1';")
        .arg(userName));
    if (query.lastError().isValid()) {
        qDebug() << "When getJoinedChatList: " << query.lastError();
        return chatList;
    }

    while (query.next()) {
        chatList.append(query.value(0).toString());
    }

    return chatList;
}

QJsonArray ServerCore::getUnjoinedChatList(const QString &userName) {
    QJsonArray chatList;
    QSqlQuery query;

    // 查询该用户未加入的所有聊天室
    query.exec(QString(
        "SELECT c_name FROM chatroom "
        "WHERE c_id NOT IN "
        "(SELECT user_chatroom.c_id FROM user_chatroom "
        "INNER JOIN chatroom ON user_chatroom.c_id = chatroom.c_id "
        "INNER JOIN user ON user_chatroom.u_id = user.u_id "
        "WHERE u_name = '%1');")
        .arg(userName));
    if (query.lastError().isValid()) {
        qDebug() << "When getUnjoinedChatList: " << query.lastError();
        return chatList;
    }

    while (query.next()) {
        chatList.append(query.value(0).toString());
    }

    return chatList;
}

QJsonArray ServerCore::getChatUserList(const QString &chatName) {
    QJsonArray userList;
    QSqlQuery query;

    // 查询该聊天室的所有用户
    query.exec(QString(
        "SELECT u_name FROM user_chatroom "
        "INNER JOIN chatroom ON user_chatroom.c_id = chatroom.c_id "
        "INNER JOIN user ON user_chatroom.u_id = user.u_id "
        "WHERE c_name = '%1';")
        .arg(chatName));
    if (query.lastError().isValid()) {
        qDebug() << query.lastError();
        return userList;
    }

    while (query.next()) {
        userList.append(query.value(0).toString());
    }

    return userList;
}

QJsonArray ServerCore::getMessage(const QString &chatName, const int latestMessageID) {
    QJsonArray messageList;
    QSqlQuery query;

    // 查询该聊天室 ID > latestMessageID 的所有消息（每条聊天记录包含id,name,content,time）
    query.exec(QString(
        "SELECT m_id, u_name, m_text, m_t FROM message "
        "INNER JOIN user ON message.m_u_id = user.u_id "
        "INNER JOIN chatroom ON message.m_c_id = chatroom.c_id "
        "WHERE c_name = '%1' AND m_id > %2 "
        "ORDER BY message.m_t ASC;")
        .arg(chatName)
        .arg(latestMessageID));
    if (query.lastError().isValid()) {
        qDebug() << query.lastError();
        return messageList;
    }

    while (query.next()) {
        QJsonObject message;
        message.insert("id", query.value(0).toInt());
        message.insert("name", query.value(1).toString());
        message.insert("content", query.value(2).toString());
        message.insert("time", query.value(3).toString());
        messageList.append(message);
    }

    return messageList;
}

bool ServerCore::sendMessage(const QString &chatName, const QString &senderName, const QString &message) {
    QSqlQuery query;

    // 查询该聊天室的 ID
    query.exec(QString(
        "SELECT c_id FROM chatroom WHERE c_name = '%1';")
        .arg(chatName));
    if (query.lastError().isValid()) {
        qDebug() << query.lastError();
        return false;
    }
    if (!query.next()) {
        qDebug() << "聊天室不存在!";
        return false;
    }
    int chatID = query.value(0).toInt();

    // 查询该用户的 ID
    query.exec(QString(
        "SELECT u_id FROM user WHERE u_name = '%1';")
        .arg(senderName));
    if (query.lastError().isValid()) {
        qDebug() << query.lastError();
        return false;
    }
    if (!query.next()) {
        qDebug() << "用户不存在!";
        return false;
    }
    int senderID = query.value(0).toInt();

    // 插入消息 m_id, m_u_id, m_c_id, m_t, m_text
    query.exec(QString(
        "INSERT INTO message (m_id, m_u_id, m_c_id, m_t, m_text) "
        "VALUES (%1,%2,%3,'%4','%5');")
        .arg(++maxMessageNumber)
        .arg(senderID)
        .arg(chatID)
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
        .arg(message));
    if (query.lastError().isValid()) {
        qDebug() << query.lastError();
        return false;
    }

    synchronizationRemind(chatName, senderName);

    return true;
}

bool ServerCore::synchronizationRemind(const QString &chatName, const QString &senderName) {
    return true;
    // 发送同步提醒消息，提醒聊天室中所有在线用户更新信息
    QJsonObject remindJsonObj = baseJsonObj("synchronization", "remind");

    // 编辑数据字段
    QJsonObject dataObj;
    dataObj["chatName"] = chatName;
    remindJsonObj["data"] = dataObj;

    // 发送消息给该聊天室的在线用户（从数据库中选出该聊天室用户，再用userSocketMap尝试发送）
    QJsonArray userList = getChatUserList(chatName);
    qDebug() << "发送同步提醒报文：" << userList;
    for (int i = 0; i < userList.size(); i++) {
        QString userName = userList.at(i).toString();
        if (userName == senderName) {   // 不给消息发送者发送同步提醒
            continue;
        }
        if (userSocketMap.contains(userName)) {
            sendJsonObj(userSocketMap[userName], remindJsonObj);
        }
    }

    return true;
}

void ServerCore::onReceiveMessage(QTcpSocket *socket, const QString &message) {
    // 解析JSON字符串
    QJsonDocument jsonDoc = QJsonDocument::fromJson(message.toUtf8());

    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        qDebug() << "数据报文解析失败!";
        return;
    }
    QJsonObject jsonObj = jsonDoc.object();

    if (jsonObj["state"].toString() != "request") {
        qDebug() << "该数据报文不是请求报文!";
        return;
    }

    QString type = jsonObj["type"].toString();
    QJsonObject dataObj = jsonObj["data"].toObject();

    if (type == "test") {
        if (dataObj["test"] == "test") {
            qDebug() << "测试成功";
            QJsonObject resJsonObj = baseJsonObj(type, "success");
            sendJsonObj(socket, resJsonObj);
        } else {
            qDebug() << "测试失败";
            QJsonObject resJsonObj = baseJsonObj(type, "failed");
            sendJsonObj(socket, resJsonObj);
            return;
        }
    } else if (type == "register") {
        if (registerAccount(dataObj["userName"].toString(), dataObj["password"].toString())) {
            qDebug() << "新账号注册成功";
            QJsonObject resJsonObj = baseJsonObj(type, "success");

            // 编辑数据字段
            QJsonObject resDataObj = resJsonObj["data"].toObject();
            resDataObj["userName"] = dataObj["userName"].toString();
            resJsonObj["data"] = resDataObj;

            sendJsonObj(socket, resJsonObj);
        } else {
            qDebug() << "新账号注册失败";
            QJsonObject resJsonObj = baseJsonObj(type, "failed");
            sendJsonObj(socket, resJsonObj);
            return;
        }
    } else if (type == "login") {
        if (loginAccount(socket, dataObj["userName"].toString(), dataObj["password"].toString())) {
            qDebug() << "用户登录成功";
            QJsonObject resJsonObj = baseJsonObj(type, "success");

            // 编辑数据字段
            QJsonObject resDataObj = resJsonObj["data"].toObject();
            resDataObj["userName"] = dataObj["userName"].toString();
            resJsonObj["data"] = resDataObj;

            sendJsonObj(socket, resJsonObj);
        }else {
            qDebug() << "用户登录失败";
            QJsonObject resJsonObj = baseJsonObj(type, "failed");
            sendJsonObj(socket, resJsonObj);
            return;
        }
    } else if (type == "createChatroom") {
        if (createChatroom(dataObj["chatName"].toString(), dataObj["creatorName"].toString())) {
            qDebug() << "创建聊天室成功";
            QJsonObject resJsonObj = baseJsonObj(type, "success");

            // 编辑数据字段
            QJsonObject resDataObj = resJsonObj["data"].toObject();
            resDataObj["chatName"] = dataObj["chatName"].toString();
            resJsonObj["data"] = resDataObj;

            sendJsonObj(socket, resJsonObj);
        } else {
            qDebug() << "创建聊天室失败";
            QJsonObject resJsonObj = baseJsonObj(type, "failed");
            sendJsonObj(socket, resJsonObj);
            return;
        }
    } else if (type == "joinChatroom") {
        if (joinChatroom(dataObj["chatName"].toString(), dataObj["userName"].toString())) {
            qDebug() << "加入聊天室成功";
            QJsonObject resJsonObj = baseJsonObj(type, "success");

            // 编辑数据字段
            QJsonObject resDataObj = resJsonObj["data"].toObject();
            resDataObj["chatName"] = dataObj["chatName"].toString();
            resDataObj["userName"] = dataObj["userName"].toString();
            resJsonObj["data"] = resDataObj;

            sendJsonObj(socket, resJsonObj);
        } else {
            qDebug() << "加入聊天室失败";
            QJsonObject resJsonObj = baseJsonObj(type, "failed");
            sendJsonObj(socket, resJsonObj);
            return;
        }
    } else if (type == "getChatList") {
        QJsonArray joinedChatList = getJoinedChatList(dataObj["userName"].toString());
        QJsonArray unjoinedChatList = getUnjoinedChatList(dataObj["userName"].toString());

        qDebug() << "获取用户可访问的聊天室列表成功";
        QJsonObject resJsonObj = baseJsonObj(type, "success");

        // 编辑数据字段
        QJsonObject resDataObj = resJsonObj["data"].toObject();
        resDataObj["userName"] = dataObj["userName"].toString();
        resDataObj["joinedChatList"] = joinedChatList;
        resDataObj["unjoinedChatList"] = unjoinedChatList;
        resJsonObj["data"] = resDataObj;

        sendJsonObj(socket, resJsonObj);
    } else if (type == "getChatUserList") {
        QJsonArray chatUserList = getChatUserList(dataObj["chatName"].toString());

        qDebug() << "获取聊天室用户列表成功";
        QJsonObject resJsonObj = baseJsonObj(type, "success");

        // 编辑数据字段
        QJsonObject resDataObj = resJsonObj["data"].toObject();
        resDataObj["chatName"] = dataObj["chatName"].toString();
        resDataObj["chatUserList"] = chatUserList;
        resJsonObj["data"] = resDataObj;

        sendJsonObj(socket, resJsonObj);
    } else if (type == "getMessage") {
        QJsonArray messageList = getMessage(dataObj["chatName"].toString(), dataObj["latestMessageID"].toInt());

        qDebug() << "获取聊天室消息成功";
        QJsonObject resJsonObj = baseJsonObj(type, "success");

        // 编辑数据字段
        QJsonObject resDataObj = resJsonObj["data"].toObject();
        resDataObj["chatName"] = dataObj["chatName"].toString();
        resDataObj["messageList"] = messageList;
        resJsonObj["data"] = resDataObj;

        sendJsonObj(socket, resJsonObj);
        qDebug() << "已发送聊天室消息到ip地址和端口" << socket->peerAddress().toString() << socket->peerPort();
    } else if (type == "sendMessage") {
        if (sendMessage(dataObj["chatName"].toString(), dataObj["senderName"].toString(), dataObj["message"].toString())) {
            qDebug() << "发送消息成功";
            QJsonObject resJsonObj = baseJsonObj(type, "success");

            // 编辑数据字段
            QJsonObject resDataObj = resJsonObj["data"].toObject();
            resDataObj["chatName"] = dataObj["chatName"].toString();
            resDataObj["senderName"] = dataObj["senderName"].toString();
            resDataObj["message"] = dataObj["message"].toString();
            resJsonObj["data"] = resDataObj;

            sendJsonObj(socket, resJsonObj);
        } else {
            qDebug() << "发送消息失败";
            QJsonObject resJsonObj = baseJsonObj(type, "failed");
            sendJsonObj(socket, resJsonObj);
            return;
        }
    } else {
        qDebug() << "数据报文类型不正确!";
        return;
    }
}

ServerCore::ServerCore() {
    maxUserNumber = 0;
    maxChatroomNumber = 0;
    maxMessageNumber = 0;
    connect(&server, &Server::receiveMessage, this, &ServerCore::onReceiveMessage);
} // 私有构造函数，确保单例

ServerCore::~ServerCore() {
    if (userTableModel) delete userTableModel;
    if (chatTableModel) delete chatTableModel;
}

QJsonObject ServerCore::baseJsonObj(const QString &type, const QString &state) {
    // 创建一个 JSON 对象
    QJsonObject jsonObj;
    jsonObj["type"] = type;
    jsonObj["state"] = state;

    // 创建一个嵌套的JSON数据对象
    QJsonObject dataObj;
    jsonObj["data"] = dataObj;

    return jsonObj;
}

void ServerCore::sendJsonObj(QTcpSocket *socket, const QJsonObject &jsonObj) {
    // 使用 QJsonDocument 生成 JSON 字符串，并发送报文
    QJsonDocument jsonDoc(jsonObj);
    QString message = jsonDoc.toJson(QJsonDocument::Compact);
//    qDebug() << "发送报文到ip地址和端口" << socket->peerAddress().toString() << socket->peerPort();
//    qDebug() << socket->write(message.toUtf8());
    server.sendMessage(socket, message);
}
