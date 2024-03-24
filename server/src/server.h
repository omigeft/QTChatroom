#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork>
#include <QMutex>
#include <QSslCertificate>
#include <QSslKey>

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server(QObject *parent = nullptr);

    void sendMessage(QSslSocket *socket, const QString &message);

    bool loadServerCertificates(const QString &certPath, const QString &keyPath);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void onReadyRead();

    void onBytesWritten(qint64 bytes);

signals:
    void receiveMessage(QSslSocket *socket, const QString &message);

private:
    QMutex mutex;
    QSslCertificate sslCertificate;
    QSslKey sslPrivateKey;
};

#endif // SERVER_H
