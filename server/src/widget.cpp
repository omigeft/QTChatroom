#include "widget.h"
#include "ui_widget.h"
#include "adminmanagement.h"
#include <QMessageBox>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
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
    core = &ServerCore::getInstance();
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_CloseButton_clicked()
{
    this->close();
}

void Widget::on_LoginButton_clicked()
{
    if (core->createServer(QHostAddress(ui->IPInput->text()), ui->PortInput->text().toInt(),
                           ui->AdminInput->text(), ui->PassWordInput->text())) {
        //跳转界面
        AdminManagement * adminManageWindow = new AdminManagement;
        this->close();
        adminManageWindow->show();
    }
}
void Widget::mousePressEvent(QMouseEvent * event)
{
    diff_pos = this->pos()-event->globalPos();
}
void Widget::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos()+diff_pos);
}
