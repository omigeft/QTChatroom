#include "server.h"

Server::Server(QObject *parent)
    : QTcpServer(parent) {}

void Server::sendMessage(QSslSocket *socket, const QString &message) {
    QByteArray data = message.toUtf8();
    mutex.lock();
    socket->write(data);
    socket->flush();
    mutex.unlock();
}

bool Server::loadServerCertificates(const QString &certPath, const QString &keyPath) {
    QFile certFile(certPath);
    QFile keyFile(keyPath);

    if (!certFile.open(QIODevice::ReadOnly) || !keyFile.open(QIODevice::ReadOnly)) {
        qWarning() << "无法打开证书或密钥文件";
        return false;
    }

    QSslCertificate certificate(&certFile, QSsl::Pem);
    QSslKey privateKey(&keyFile, QSsl::Rsa, QSsl::Pem);

    if (certificate.isNull() || privateKey.isNull()) {
        qWarning() << "无法加载证书或密钥";
        return false;
    }

    sslCertificate = certificate;
    sslPrivateKey = privateKey;

    return true;
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    // 创建一个新的SslSocket来处理连接
    QSslSocket *clientSocket = new QSslSocket(this);
    if (clientSocket->setSocketDescriptor(socketDescriptor))
    {
        addPendingConnection(clientSocket);
        clientSocket->setLocalCertificate(sslCertificate);
        clientSocket->setPrivateKey(sslPrivateKey);
        connect(clientSocket, &QSslSocket::readyRead, this, &Server::onReadyRead);
        connect(clientSocket, &QSslSocket::bytesWritten, this, &Server::onBytesWritten);
        connect(clientSocket, &QSslSocket::disconnected, clientSocket, &QSslSocket::deleteLater);
        clientSocket->startServerEncryption();
    }
    else
    {
        delete clientSocket;
    }
}

void Server::onReadyRead()
{
    QSslSocket *clientSocket = qobject_cast<QSslSocket *>(sender());
    if (clientSocket && clientSocket->bytesAvailable())
    {
        QByteArray data = clientSocket->readAll();
        QString dataString = QString::fromUtf8(data);
        qDebug() << "收到数据:" << dataString;
        emit receiveMessage(clientSocket, dataString);
    }
}

void Server::onBytesWritten(qint64 bytes)
{
//    QSslSocket *clientSocket = qobject_cast<QSslSocket *>(sender());
    qDebug() << "发送数据:" << bytes << "字节";
}

