#ifndef CLIENTCORE_H
#define CLIENTCORE_H

#include <QtNetwork>
#include <QJsonObject>
#include <QJsonDocument>
class ClientCore : public QObject
{
public:
    static ClientCore& getInstance() {
        static ClientCore instance; // 单例对象
        return instance;
    }

    bool connectServer(const QHostAddress &address, const quint16 &port);

    bool registerRequest(const QString &userName, const QString &password);

    bool loginRequest(const QString &userName, const QString &password);

    bool createChatroomRequest(const QString &chatName, const QString &creatorName);

    bool getChatListRequest(const QString &userName);

private:
    ClientCore(); // 私有构造函数，确保单例

    bool checkResponseMessage(const QString &message, const QString &type);

    QJsonObject baseJsonObj(const QString &type, const QString &state);

    void sendJsonObj(const QJsonObject &jsonObj);

public:
    QTcpSocket socket;          // 套接字
    QHostAddress serverAddress; // 服务器IP地址
    quint16 serverPort;         // 服务器端口

    QString currentUserName;    // 当前登录的用户名

    QStringList joinedList;
    QStringList unjoinedList;
    QStringList selectList;
};


#endif // CLIENTCORE_H
