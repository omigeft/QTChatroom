#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork>

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server(QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void onReadyRead();

signals:
    void receiveMessage(const QString &message);
};

#endif // SERVER_H
