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

//    //加载已加入的聊天室
//    Joinlist<<"加入的第1个聊天室"<<"加入的第2个聊天室"<<"加入的第3个聊天室";
//    ui->HJoinChatListWidget->addItems(Joinlist);
//    ui->HJoinChatListWidget->setCurrentRow(0);
//
//    //加载未加入的聊天室
//    UJoinlist<<"未加入的第1个聊天室"<<"未加入的第2个聊天室"<<"未加入的第3个聊天室";
//    ui->UJoinChatListWidget->addItems(UJoinlist);
//    ui->UJoinChatListWidget->setCurrentRow(0);
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
    if(chatName!=""){
    //创建新聊天并及加入到列表
    Joinlist<<chatName;
    ui->HJoinChatListWidget->addItem(chatName);
    qDebug()<<chatName;
    //将注册信息发到服务器
    }
    else
        QMessageBox::information(this,"群聊名有误","群聊名不能为空");

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
