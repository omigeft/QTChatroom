#include "server.h"

Server::Server(QObject *parent)
    : QTcpServer(parent) {}

void Server::incomingConnection(qintptr socketDescriptor)
{
    // 创建一个新的TcpSocket来处理连接
    QTcpSocket *clientSocket = new QTcpSocket(this);
    if (clientSocket->setSocketDescriptor(socketDescriptor))
    {
        this->addPendingConnection(clientSocket);
        connect(clientSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        connect(clientSocket, SIGNAL(disconnected()), clientSocket, SLOT(deleteLater()));
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
        clientSocket->write("Data received!");   // 简单回复
        emit receiveMessage(dataString);
    }
}