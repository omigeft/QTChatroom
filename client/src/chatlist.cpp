#include "chatlist.h"
#include <QWidget>
ChatList::ChatList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatList)
{
    ui->setupUi(this);

    // 获取核心实例
    core = &ClientCore::getInstance();

    this->setWindowFlags(Qt::FramelessWindowHint);//去除标题栏
    this->setAttribute(Qt::WA_TranslucentBackground);//透明
    //设置该聊天窗口的标题-----用户名
    ui->UserNameLabel->setText("当前用户：" + core->currentUserName);
    //设置背景
    //QPalette palette = ui->frame->palette();
    QString qss = "#frame{border-image:url(:/pic/listback"+QString::number(QRandomGenerator::global()->bounded(4))+".jpg)}";
    ui->frame->setStyleSheet(qss);

    //palette.setBrush(QPalette::Window,QBrush(QPixmap(":/pic/listback"+QString::number(QRandomGenerator::global()->bounded(4))+".jpg")));
    //this->setPalette(palette);
    //设置关闭按钮
    ui->closeButton->setIcon(QPixmap(":/icon/icon/close.png"));
    // 刷新列表
    refreshChatList();
}

ChatList::~ChatList()
{
    delete ui;
}

void ChatList::refreshChatList()
{
    // 从服务器上获取一次聊天室列表
    core->getChatListRequest(core->currentUserName);

    // 清空列表
    ui->HJoinChatListWidget->clear();
    ui->UJoinChatListWidget->clear();

    // 更新列表
    ui->HJoinChatListWidget->addItems(core->joinedList);
    ui->UJoinChatListWidget->addItems(core->unjoinedList);
}

void ChatList::on_OpenChatButton_clicked()
{
    if(ui->HJoinChatListWidget->count()>0)
    {
        QString chatName = ui->HJoinChatListWidget->currentItem()->text();
        qDebug()<<chatName;

        //打开聊天界面
        Chat * userChat = new Chat(chatName);
        userChat->show();
        core->nameChatMap.insert(chatName, userChat);
    }
}

void ChatList::on_JoinButton_clicked()
{
    if(ui->UJoinChatListWidget->count()>0)
    {
        QString chatName = ui->UJoinChatListWidget->currentItem()->text();
        core->joinChatroomRequest(chatName, core->currentUserName);
        refreshChatList();
    }
}

void ChatList::on_NewChatButton_clicked()
{
    QString chatName = ui->ChatNameInput->text();
    if (chatName.length() > 0 && chatName.length() <= 20) {
        core->createChatroomRequest(chatName, core->currentUserName);
        refreshChatList();
    } else {
        QMessageBox::information(this,"群聊名有误","群聊名长度应在1~20个字符之间");
    }
}

void ChatList::on_SortChatButton_clicked()
{
    //筛选，包含部分的都会筛选出来
    QString chatName = ui->ChatNameInput->text();
    core->selectList=core->unjoinedList.filter(chatName);
    ui->UJoinChatListWidget->clear();
    ui->UJoinChatListWidget->addItems(core->selectList);
    ui->UJoinChatListWidget->setCurrentRow(0);
}

void ChatList::on_closeButton_clicked()
{
    this->close();
}
void ChatList::mousePressEvent(QMouseEvent * event)
{
    diff_pos = this->pos()-event->globalPos();
}
void ChatList::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos()+diff_pos);
}
