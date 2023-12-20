#include "server.h"

Server::Server(QObject *parent)
    : QTcpServer(parent) {}

void Server::sendMessage(QTcpSocket *socket, const QString &message) {
    QByteArray data = message.toUtf8();
    mutex.lock();
    socket->write(data);
    socket->flush();
    mutex.unlock();
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    // 创建一个新的TcpSocket来处理连接
    QTcpSocket *clientSocket = new QTcpSocket(this);
    if (clientSocket->setSocketDescriptor(socketDescriptor))
    {
        this->addPendingConnection(clientSocket);
        connect(clientSocket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
        connect(clientSocket, &QTcpSocket::bytesWritten, this, &Server::onBytesWritten);
        connect(clientSocket, &QTcpSocket::disconnected, clientSocket, &QTcpSocket::deleteLater);
    }
    else
    {
        delete clientSocket;
    }
}

void Server::onReadyRead()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
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
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    qDebug() << "发送数据:" << bytes << "字节";
}