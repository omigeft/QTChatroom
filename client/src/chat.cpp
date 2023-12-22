#include "chat.h"
#include <QGraphicsDropShadowEffect>
Chat::Chat(const QString &chatName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chat),
    currentChatName(chatName)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose,false); //关闭窗口时不退出程序
    ui->chatNamelabel->setText("当前聊天:" + currentChatName);

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
    // 群头像
    QString picPath = ":/pic/"+QString::number(QRandomGenerator::global()->bounded(10))+".jpg";
    qDebug()<<picPath;
    ui->Imagelabel->setPixmap(QPixmap(picPath));
    //设置关闭按钮
    ui->CloseButton->setIcon(QPixmap(":/icon/icon/close.png"));

    latestMessageID = 0;

    refreshUserList();
    refreshChat();

    // 每隔1秒重复刷新一次聊天记录
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Chat::refreshChat);
    timer->start(1000);

    // 每隔5秒重复刷新一次聊天室成员列表
    QTimer *timer2 = new QTimer(this);
    connect(timer2, &QTimer::timeout, this, &Chat::refreshUserList);
    timer2->start(5000);
}

Chat::~Chat()
{
    delete ui;
}

void Chat::refreshUserList() {
    // 从服务器上获取一次聊天室成员列表
    QJsonArray userListArray = core->getChatUserListRequest(currentChatName);
    userList.clear();
    for (int i = 0; i < userListArray.size(); i++) {
        userList.append(userListArray[i].toString());
        qDebug() << i << userListArray[i].toString();
    }

    // 清空聊天室成员列表
    ui->UserlistWidget->clear();

    // 更新聊天室成员列表
    ui->UserlistWidget->addItems(userList);
}

void Chat::refreshChat()
{
    // 从服务器上获取一次最新聊天记录
    QJsonArray newMessageArray = core->getMessageRequest(currentChatName, latestMessageID);

    // 更新latestMessageID
    if (newMessageArray.size() > 0) {
        latestMessageID = newMessageArray[newMessageArray.size() - 1].toObject()["id"].toInt();
    }

    // 更新QStringList chatHistory和显示的聊天记录
    for (int i = 0; i < newMessageArray.size(); i++) {
        QJsonObject message = newMessageArray[i].toObject();
        QString id = QString::number(message["id"].toInt());
        QString name = message["name"].toString();
        QString content = message["content"].toString();
        QString time = message["time"].toString();
        chatHistory.append("用户:" + name + " [" + time + "]");
        chatHistory.append("->" + content);
        ui->chatBrowser->append("用户:" + name + " [" + time + "]");
        ui->chatBrowser->append("->" + content);
    }

    // textBrowser自动滚动到最底部
    ui->chatBrowser->moveCursor(QTextCursor::End);
}

void Chat::on_CloseButton_clicked()
{
    this->close();
}

void Chat::on_SendButton_clicked()
{
    // 获取输入的消息
    QString message=ui->MessageInput->toPlainText();

    // 检查消息长度是否在1~200之间
    if (message.length() < 1 || message.length() > 200) {
        qDebug() << "消息长度不符合要求";
        return;
    }

    // 发送消息
    core->sendMessageRequest(currentChatName, core->currentUserName, message);

    // 清空输入框
    ui->MessageInput->clear();

    refreshChat();
}
void Chat::mousePressEvent(QMouseEvent * event)
{
    diff_pos = this->pos()-event->globalPos();
}
void Chat::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos()+diff_pos);
}
