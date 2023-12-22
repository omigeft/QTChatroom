#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwindow.h"
#include "messagebox.h"
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
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
    core = &ClientCore::getInstance();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_CloseButton_clicked()
{
    this->close();
}

void MainWindow::on_ConnectButton_clicked()
{
    // 根据IP和端口进行连接
    if (core->connectServer(QHostAddress(ui->IPInput->text()), ui->PortInput->text().toInt())) {
        //进入登录界面
        LoginWindow * loginwindow = new LoginWindow();
        this->close();
        loginwindow->show();
    } else {
        MessageBox::critical(this, "错误", "连接失败");
    }
}
void MainWindow::mousePressEvent(QMouseEvent * event)
{
    diff_pos = this->pos()-event->globalPos();
}
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos()+diff_pos);
}
