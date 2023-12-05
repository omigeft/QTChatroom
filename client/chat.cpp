#include "chat.h"
#include "ui_chat.h"
#include <QString>
#include <QRandomGenerator>
#include <QDebug>
#include <QDateTime>

Chat::Chat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chat)
{
    ui->setupUi(this);
    //伴随父窗口关闭
    setAttribute(Qt::WA_QuitOnClose,false);
}

Chat::~Chat()
{
    delete ui;
}
void Chat::ChatInit(QString username,QString chatname)
{
    UserName=username;
    ChatName=chatname;
    this->setWindowTitle("当前聊天:"+chatname);
    QString picPath = ":/pic/"+QString::number(QRandomGenerator::global()->bounded(10))+".jpg";
    qDebug()<<picPath;
    ui->Imagelabel->setPixmap(QPixmap(picPath));
    //查找聊天室成员
    userlist<<"user1"<<"user2"<<"user3";
    ui->UserlistWidget->addItems(userlist);
}

void Chat::on_CloseButton_clicked()
{
    this->close();
}

void Chat::on_SendButton_clicked()
{
    //获取发送时间
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH: mm: ss");
    //获取输入的消息
    QString Message=ui->MessageInput->toPlainText();
    qDebug()<<Message;
    //
    //发送消息
    //
    //清空输入框
    ui->MessageInput->setPlainText("");
    //输入的消息放入聊天显示
    AddMessage(UserName,Message,currentDateTime);
}
void Chat::AddMessage(QString name,QString Message,QString time)
{
    ui->chatBrowser->append("用户:"+name+" ["+time+"]");
    ui->chatBrowser->append("->"+Message);
}
void Chat::updateChatHistory()
{
    //获取该聊天室的历史记录
    //使用AddMessage()显示出来
}
