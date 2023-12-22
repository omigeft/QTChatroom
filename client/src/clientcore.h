#ifndef CLIENTCORE_H
#define CLIENTCORE_H

#include <QtNetwork>
#include <QJsonObject>
#include <QJsonDocument>
#include <QEventLoop>

class Chat;

class ClientCore : public QObject
{
    Q_OBJECT

public:
    static ClientCore& getInstance() {
        static ClientCore instance; // 单例对象
        return instance;
    }

    bool connectServer(const QHostAddress &address, const quint16 &port);

    bool registerRequest(const QString &userName, const QString &password, const QString &role);

    bool loginRequest(const QString &userName, const QString &password);

    bool createChatroomRequest(const QString &chatName, const QString &creatorName);

    bool joinChatroomRequest(const QString &chatName, const QString &userName);

    bool getChatListRequest(const QString &userName);

    QJsonArray getChatUserListRequest(const QString &chatName);

    QJsonArray getMessageRequest(const QString &chatName, const int latestMessageID, const QString &lastTime);

    bool sendMessageRequest(const QString &chatName, const QString &senderName, const QString &message);

    void processReadMessage(const QString &message);

signals:
    void readMessage(const QString &message);

    void remindSignal(const QString &chatName);

private slots:
    void onReadyRead();

private:
    ClientCore(); // 私有构造函数，确保单例

    bool checkMessage(const QString &message, const QString &type, const QString &state);

    QJsonObject baseJsonObj(const QString &type, const QString &state);

    void sendJsonObj(const QJsonObject &jsonObj);

    bool sendAndWait(QString &response, const QJsonObject &jsonObj);

public:
    QTcpSocket socket;          // 套接字
    QHostAddress serverAddress; // 服务器IP地址
    quint16 serverPort;         // 服务器端口

    QString currentUserName;    // 当前登录的用户名

    QStringList joinedList;
    QStringList unjoinedList;
    QStringList selectList;

    QMap<QString, Chat*> nameChatMap; // 打开的聊天室的聊天室名与窗口实例的映射
};


#endif // CLIENTCORE_H
