#ifndef SERVERCORE_H
#define SERVERCORE_H

#include <QtNetwork>
#include <QtSql>
#include <QTableView>
#include "server.h"

class ServerCore : public QObject
{
    Q_OBJECT

public:
    static ServerCore& getInstance() {
        static ServerCore instance; // 单例对象
        return instance;
    }

public:
    bool createServer(QHostAddress address, quint16 port);

    bool createDatabase();

    bool registerAccount(const QString &userName, const QString &password);

    bool loginAccount(QTcpSocket *socket, const QString &userName, const QString &password);

    bool createChatroom(const QString &chatroomName, const QString &userName);

    QJsonArray getJoinedChatList(const QString &userName);

    QJsonArray getUnjoinedChatList(const QString &userName);

    QJsonArray getChatUserList(const QString &chatName);

    QJsonArray getMessage(const QString &chatName, const int latestMessageID);

    bool sendMessage(const QString &chatName, const QString &senderName, const QString &message);

    bool synchronizationRemind(const QString &chatName);

private slots:
    void onReceiveMessage(QTcpSocket *socket, const QString &message);

private:
    ServerCore(); // 私有构造函数，确保单例

    ~ServerCore();

    QJsonObject baseJsonObj(const QString &type, const QString &state);

    void sendJsonObj(QTcpSocket *socket, const QJsonObject &jsonObj);

public:
    Server server;                  // 服务器
    QHostAddress serverAddress;     // 服务器IP地址
    quint16 serverPort;             // 服务器端口

    QSqlDatabase db;                // 数据库
    QSqlTableModel* userTableModel;
    QSqlTableModel* chatTableModel;

    QMap<QString, QTcpSocket*> userSocketMap; // 用户名与socket的映射

private:
    int maxUserNumber;              // 用于计数累计用户数量，从而确定新建u_id
    int maxChatroomNumber;          // 用于计数累计聊天室数量，从而确定新建c_id
    int maxMessageNumber;           // 用于计数累计消息数量，从而确定新建m_id
};

#endif // SERVERCORE_H
