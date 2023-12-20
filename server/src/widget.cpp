#include "widget.h"
#include "ui_widget.h"
#include "adminmanagement.h"
#include <QMessageBox>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

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

    // //判断是否正确验证
    // bool isRight;
    // isRight = true;
    // if(isRight)
    // {
    //     //跳转界面
    //     AdminManagement * adminManageWindow = new AdminManagement;
    //     this->close();
    //     adminManageWindow->show();
    // }
    // else
    // {
    //     //验证错误
    //     QMessageBox::information(this,"输入有误","请输入正确的IP地址、端口号、账户名、密码");
    // }
}
