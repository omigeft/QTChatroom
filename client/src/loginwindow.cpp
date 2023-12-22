#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "mainwindow.h"
#include "chatlist.h"
#include "messagebox.h"
#include <QPainter>
#include <QGraphicsDropShadowEffect>
LoginWindow::LoginWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWindow) {
    ui->setupUi(this);
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
    // 获取核心实例
    core = &ClientCore::getInstance();
}

LoginWindow::~LoginWindow() {
    delete ui;
}

void LoginWindow::on_CloseButton_clicked() {
    this->close();
}

void LoginWindow::on_RegisterButton_clicked() {
    if (core->registerRequest(ui->NameInput->text(), ui->PassWordInput->text(), "user")) {
        MessageBox::information(this, "提示", "注册成功");
    } else {
        MessageBox::critical(this, "错误", "注册失败");
    }
}

void LoginWindow::on_LoginButton_clicked()
{
    if (core->loginRequest(ui->NameInput->text(), ui->PassWordInput->text())) {
        //切换到用户列表
        ChatList * userChatList = new ChatList();
        this->close();
        userChatList->show();
    } else {
        MessageBox::critical(this, "错误", "登录失败");
    }
}

void LoginWindow::paintEvent(QPaintEvent *) {
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void LoginWindow::mousePressEvent(QMouseEvent * event) {
    diff_pos = this->pos()-event->globalPos();
}

void LoginWindow::mouseMoveEvent(QMouseEvent *event) {
    this->move(event->globalPos()+diff_pos);
}
