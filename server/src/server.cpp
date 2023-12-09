#include "server.h"

Server::Server(QObject *parent)
    : QTcpServer(parent) {}

void Server::incomingConnection(qintptr socketDescriptor)
{
    // 创建一个新的TcpSocket来处理连接
    QTcpSocket *clientSocket = new QTcpSocket(this);
    if (clientSocket->setSocketDescriptor(socketDescriptor))
    {
        connect(clientSocket, SIGNAL(readyRead()), this, SLOT(readClient()));
        connect(clientSocket, SIGNAL(disconnected()), clientSocket, SLOT(deleteLater()));
    }
    else
    {
        delete clientSocket;
    }
}

void Server::readClient()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (clientSocket && clientSocket->bytesAvailable())
    {
        QByteArray data = clientSocket->readAll();
        // 在这里你可以处理客户端发送的数据，这里我们简单地回显消息
        clientSocket->write(data);
    }
}
