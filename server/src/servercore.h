#ifndef SERVERCORE_H
#define SERVERCORE_H

#include <QtNetwork>
#include <QtSql>
#include <QTableView>
#include "server.h"

class ServerCore : public QObject
{
public:
    static ServerCore& getInstance() {
        static ServerCore instance; // 单例对象
        return instance;
    }

public:
    bool createServer(QHostAddress address, quint16 port);

    bool createDatabase();

    bool registerAccount(const QString &userName, const QString &password);

private slots:
    void onReceiveMessage(const QString &message);

private:
    ServerCore(); // 私有构造函数，确保单例

    ~ServerCore();

public:
    Server server;                  // 服务器
    QHostAddress serverAddress;     // 服务器IP地址
    quint16 serverPort;             // 服务器端口

    QSqlDatabase db;                // 数据库
    QSqlTableModel* userTableModel;
    QSqlTableModel* chatTableModel;

private:
    int maxUserNumber;                 // 用于计数累计用户数量，从而确定新建u_id
};

#endif // SERVERCORE_H
