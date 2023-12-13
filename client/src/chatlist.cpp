#include "chatlist.h"
ChatList::ChatList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatList)
{
    ui->setupUi(this);

    // 获取核心实例
    core = &ClientCore::getInstance();
}

ChatList::~ChatList()
{
    delete ui;
}

void ChatList::initChatList()
{
    //设置该聊天窗口的标题-----用户名
    this->setWindowTitle("当前用户：" + core->currentUserName);
}

void ChatList::on_OpenChatButton_clicked()
{
    if(ui->HJoinChatListWidget->count()>0)
    {
        QString chatName = ui->HJoinChatListWidget->currentItem()->text();
        qDebug()<<chatName;

        //打开聊天界面
        Chat * userChat = new Chat;
        userChat->ChatInit(core->currentUserName,chatName);
        userChat->show();
    }
}

void ChatList::on_JoinButton_clicked()
{
    if(ui->UJoinChatListWidget->count()>0)
    {
        QString chatName = ui->UJoinChatListWidget->currentItem()->text();
        qDebug()<<chatName;
        //将选中的聊天加入,并发送添加和更新信息
        Joinlist<<chatName;
        ui->HJoinChatListWidget->addItem(chatName);
    }
}

void ChatList::on_NewChatButton_clicked()
{
    QString chatName = ui->ChatNameInput->text();
    if (chatName.length() > 0 && chatName.length() <= 20) {
        core->createChatroomRequest(ui->ChatNameInput->text(), core->currentUserName);
    } else {
        QMessageBox::information(this,"群聊名有误","群聊名长度应在1~20个字符之间");
    }

}

void ChatList::on_SortChatButton_clicked()
{
    //筛选，包含部分的都会筛选出来
    QString chatName = ui->ChatNameInput->text();
    selectlist=UJoinlist.filter(chatName);
    ui->UJoinChatListWidget->clear();
    ui->UJoinChatListWidget->addItems(selectlist);
    ui->UJoinChatListWidget->setCurrentRow(0);
}
