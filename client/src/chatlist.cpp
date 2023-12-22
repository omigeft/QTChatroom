#include "chatlist.h"
#include "messagebox.h"
#include <QWidget>
#include <QGraphicsDropShadowEffect>
ChatList::ChatList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatList) {
    ui->setupUi(this);

    // 获取核心实例
    core = &ClientCore::getInstance();

    this->setWindowFlags(Qt::FramelessWindowHint);//去除标题栏
    this->setAttribute(Qt::WA_TranslucentBackground);//透明
    //绘制阴影
    QGraphicsDropShadowEffect * shadowEffect = new QGraphicsDropShadowEffect();
    shadowEffect->setOffset(0, 0);
    QColor color = Qt::black;
    color.setAlpha(64);
    shadowEffect->setColor(color);
    shadowEffect->setBlurRadius(20);
    ui->frame->setGraphicsEffect(shadowEffect);
    //设置该聊天窗口的标题-----用户名
    ui->UserNameLabel->setText("当前用户：" + core->currentUserName);
    //设置背景
    //QString qss = "#frame{border-image:url(:/pic/listback"+QString::number(QRandomGenerator::global()->bounded(4))+".jpg)}";
    //ui->frame->setStyleSheet(qss);
    //设置关闭按钮
    ui->closeButton->setIcon(QPixmap(":/icon/icon/close.png"));

    ui->NewChatButton->setEnabled(false);

    // 如果ui->ChatNameInput编辑了不为空，则激活ui->NewChatButton
    connect(ui->ChatNameInput, &QLineEdit::textChanged, [=](){
        ui->NewChatButton->setEnabled(!ui->ChatNameInput->text().isEmpty());
    });

    // 刷新列表
    refreshChatList();

    // 每隔5秒重复刷新一次聊天室列表
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ChatList::refreshChatList);
    timer->start(5000);
}

ChatList::~ChatList() {
    delete ui;
}

void ChatList::refreshChatList() {
    // 从服务器上获取一次聊天室列表
    core->getChatListRequest(core->currentUserName);

    // 清空列表
    ui->HJoinChatListWidget->clear();
    ui->UJoinChatListWidget->clear();

    // 更新列表
    if (findName.isEmpty()) {
        ui->HJoinChatListWidget->addItems(core->joinedList);
        ui->UJoinChatListWidget->addItems(core->unjoinedList);
    } else {
        for (auto it = core->joinedList.begin(); it != core->joinedList.end(); it++) {
            if (it->contains(findName)) {
                ui->HJoinChatListWidget->addItem(*it);
            }
        }
        for (auto it = core->unjoinedList.begin(); it != core->unjoinedList.end(); it++) {
            if (it->contains(findName)) {
                ui->UJoinChatListWidget->addItem(*it);
            }
        }
    }
}

void ChatList::on_OpenChatButton_clicked() {
    if (ui->HJoinChatListWidget->count() > 0 && ui->HJoinChatListWidget->currentItem() != NULL) {
        qDebug() << ui->HJoinChatListWidget->count();
        QString chatName = ui->HJoinChatListWidget->currentItem()->text();
        qDebug() << chatName;

        //打开聊天界面
        Chat * userChat = new Chat(chatName);
        userChat->show();
        core->nameChatMap.insert(chatName, userChat);
    } else {
        MessageBox::critical(this, "错误", "打开失败，请检查是否选中聊天室");
    }
}

void ChatList::on_JoinButton_clicked() {
    if (ui->UJoinChatListWidget->count() > 0 && ui->UJoinChatListWidget->currentItem() != NULL) {
        QString chatName = ui->UJoinChatListWidget->currentItem()->text();
        core->joinChatroomRequest(chatName, core->currentUserName);
        refreshChatList();
    } else {
        MessageBox::critical(this, "错误", "加入失败，请检查是否选中聊天室");
    }
}

void ChatList::on_NewChatButton_clicked() {
    QString chatName = ui->ChatNameInput->text();
    if (chatName.length() > 0 && chatName.length() <= 20) {
        core->createChatroomRequest(chatName, core->currentUserName);
        refreshChatList();
    } else {
        QMessageBox::critical(this, "错误", "群聊名长度应在1~20个字符之间");
    }
}

void ChatList::on_FindChatButton_clicked() {
    findName = ui->ChatNameInput->text();
    refreshChatList();
}

void ChatList::on_closeButton_clicked() {
    this->close();
}
void ChatList::mousePressEvent(QMouseEvent * event) {
    diff_pos = this->pos()-event->globalPos();
}
void ChatList::mouseMoveEvent(QMouseEvent *event) {
    this->move(event->globalPos()+diff_pos);
}
