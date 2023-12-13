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
    if (!query.exec(
                "CREATE TABLE IF NOT EXISTS user ("
                "u_id    INT PRIMARY KEY,"              // user id
                "u_name  VARCHAR(20) UNIQUE NOT NULL,"  // user name
                "pw      VARCHAR(20) NOT NULL,"         // password
                "su_t    DATETIME,"                     // sign up time
                "sd_t    DATETIME);"                    // sign down time
                ))
        return false;

    if (!query.exec(
                "CREATE TABLE IF NOT EXISTS chatroom ("
                "c_id    INT PRIMARY KEY,"              // chatroom id
                "c_name  VARCHAR(20) UNIQUE NOT NULL,"  // chatroom name
                "c_u_id  INT NOT NULL,"                 // creator user id
                "cr_t    DATETIME,"                     // creation time
                "ds_t    DATETIME);"                    // dissolution time
                ))
        return false;

    query.exec("SELECT MAX(u_id) FROM user");
    query.next();
    maxUserNumber = query.value(0).toInt(); // 获取用户号最大值

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

    //qDebug() << sql_statement;

//    qDebug() << query.exec("SELECT * FROM user;");

//    while (query.next()) {
//        QSqlRecord record = query.record();
//        int columnCount = record.count();

//        for (int i = 0; i < columnCount; ++i) {
//            qDebug() << record.fieldName(i) << ":" << query.value(i);
//        }
//    }

    // 执行SQL语句
    if (!query.exec(sql_statement))
        return false;

    // 更新界面显示的用户表
    userTableModel->select();

    return true;
}

void ServerCore::onReceiveMessage(const QString &message) {
    // 解析JSON字符串
    QJsonDocument jsonDoc = QJsonDocument::fromJson(message.toUtf8());

    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        QJsonObject jsonObj = jsonDoc.object();

        if (jsonObj.contains("type") && jsonObj["type"].isString()) {
            QString type = jsonObj["type"].toString();
            QJsonObject dataObj = jsonObj["data"].toObject();
            qDebug() << "Type:" << type;

            if (type == "register") {
                if (registerAccount(dataObj["userName"].toString(), dataObj["password"].toString()))
                    qDebug() << "新账号注册成功";
                else
                    qDebug() << "新账号注册失败";
            }

        } else {
            qDebug() << "无法确定数据报文的类型!";
        }
    } else {
        qDebug() << "数据报文解析失败!";
    }
}

ServerCore::ServerCore() {
    maxUserNumber = 0;
    connect(&server, &Server::receiveMessage, this, &ServerCore::onReceiveMessage);
} // 私有构造函数，确保单例

ServerCore::~ServerCore() {
    if (userTableModel) delete userTableModel;
    if (chatTableModel) delete chatTableModel;
}