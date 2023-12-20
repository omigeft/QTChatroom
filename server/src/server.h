#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork>
#include <QMutex>

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server(QObject *parent = nullptr);

    void sendMessage(QTcpSocket *socket, const QString &message);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void onReadyRead();

    void onBytesWritten(qint64 bytes);

signals:
    void receiveMessage(QTcpSocket *socket, const QString &message);

private:
    QMutex mutex;
};

#endif // SERVER_H
