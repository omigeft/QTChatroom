#include "usermanage.h"
#include "ui_usermanage.h"

UserManage::UserManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserManage)
{
    ui->setupUi(this);
}

UserManage::~UserManage()
{
    delete ui;
}
