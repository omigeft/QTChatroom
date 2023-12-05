#include "messagemanage.h"
#include "ui_messagemanage.h"

MessageManage::MessageManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessageManage)
{
    ui->setupUi(this);
}

MessageManage::~MessageManage()
{
    delete ui;
}
