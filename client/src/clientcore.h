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

    bool connectServer(QHostAddress address, quint16 port);

    bool registerRequest(QString userName, QString password);

    bool loginRequest(QString userName, QString password);

private:
    ClientCore(); // 私有构造函数，确保单例

    bool checkResponseMessage(QString message, QString type);

    QJsonObject baseJsonObj(const QString &type, const QString &state);

    void sendJsonObj(const QJsonObject &jsonObj);

public:
    QTcpSocket socket;          // 套接字
    QHostAddress serverAddress; // 服务器IP地址
    quint16 serverPort;         // 服务器端口

    QString currentUserName;    // 当前登录的用户名
};


#endif // CLIENTCORE_H
