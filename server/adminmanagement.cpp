#include "adminmanagement.h"
#include "ui_adminmanagement.h"

AdminManagement::AdminManagement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminManagement)
{
    ui->setupUi(this);
}

AdminManagement::~AdminManagement()
{
    delete ui;
}
