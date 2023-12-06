#ifndef PUBLICDATA_H
#define PUBLICDATA_H

#include <QtNetwork>

class PublicData
{
public:
    static PublicData& getInstance() {
        static PublicData instance; // 单例对象
        return instance;
    }

public:
    QTcpSocket socket;      // 套接字
    QString serverAddress;  // 服务器IP地址
    quint16 serverPort;     // 服务器端口

private:
    PublicData() {} // 私有构造函数，确保单例
};


#endif // PUBLICDATA_H
