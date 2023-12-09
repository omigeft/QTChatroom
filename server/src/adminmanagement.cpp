#include "adminmanagement.h"
#include "ui_adminmanagement.h"
#include "usermanage.h"
#include "messagemanage.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QTableView>
#include <QMessageBox>
#include <QtDebug>
AdminManagement::AdminManagement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminManagement)
{
    ui->setupUi(this);

    // 获取核心实例
    core = &ServerCore::getInstance();

    ui->UserTable->setModel(core->userTableModel);
    ui->ChatTable->setModel(core->chatTableModel);

    //设置表格数据区内的所有单元格都不允许编辑
//    ui->UserTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    ui->ChatTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
//     //行头自适应表格
//    ui->UserTable->horizontalHeader()->setStretchLastSection(true);
//    ui->ChatTable->horizontalHeader()->setStretchLastSection(true);
//    //设置表格中每一行的表头
//    ui->UserTable->setHorizontalHeaderLabels(QStringList() << "ID" << "用户名" << "密码"<<"注册时间");
//    ui->ChatTable->setHorizontalHeaderLabels(QStringList() << "ID" << "名称" << "创建者ID"<<"创建时间");

//    //测试使用
//    insertUserIitem(0,0,"C语言");
//    insertUserIitem(0,1,"http://c.tian.net/c/");
//    insertUserIitem(0,2,"已更新");
//    insertUserIitem(0,3,"更新");

//    insertChatIitem(0,0,"1234");
//    insertChatIitem(0,1,"567123456464");
//    insertChatIitem(0,2,"8");
//    insertChatIitem(0,3,"9");
//    insertChatIitem(1,0,"134");
//    insertChatIitem(1,1,"");
//    insertChatIitem(2,0,"124");
}

AdminManagement::~AdminManagement()
{
    delete ui;
}
void AdminManagement::insertUserIitem(int row,int column,QString content)
{
//    ui->UserTable->setItem(row,column,new QTableWidgetItem(content));
}
void AdminManagement::insertChatIitem(int row,int column,QString content)
{
//    ui->ChatTable->setItem(row,column,new QTableWidgetItem(content));
}
void AdminManagement::on_ChangeButton_clicked()
{



//    if(QMessageBox::Yes==QMessageBox::question(this,"再次确认","再次确认是否修改")){
//    ui->UserTable->currentItem()->setText(ui->ContentInput->text());
//    ui->ContentInput->text().clear();
//    }
    //发送修改信息
}

void AdminManagement::on_DeleteButton_clicked()
{
//    if(QMessageBox::Yes==QMessageBox::question(this,"再次确认","再次确认是否删除")){
//        int curRow=ui->UserTable->currentRow();     //当前行号
//        ui->UserTable->removeRow(curRow);           //删除当前行及其items
//    }
    //发送删除信息
}

void AdminManagement::on_FindButton_clicked()
{
//    QList<QTableWidgetItem *> findItemsList = ui->UserTable->findItems(ui->ContentInput->text(),Qt::MatchContains);
//    if (ui->ContentInput->text()==""){//判断是否是空，如果是空就显示所有行
//       for(int i=0;i<ui->UserTable->rowCount();i++)
//           ui->UserTable->setRowHidden(i,false);//为false就是显示
//    }
//    else{
//    //然后把所有行都隐藏
//    for(int i=0;i<ui->UserTable->rowCount();i++)
//        ui->UserTable->setRowHidden(i,true);//隐藏
//    //判断符合条件索引是不是空
//    if(!findItemsList.empty())//恢复对应的行
//      for(int i=0;i<findItemsList.count();i++)
//         ui->UserTable->setRowHidden(findItemsList.at(i)->row(),false);//回复对应的行，也可以回复列
//    }
}

void AdminManagement::on_FindButton_2_clicked()
{
//    QList<QTableWidgetItem *> findItemsList = ui->ChatTable->findItems(ui->ChatInput->text(),Qt::MatchContains);
//    if (ui->ChatInput->text()==""){//判断是否是空，如果是空就显示所有行
//       for(int i=0;i<ui->ChatTable->rowCount();i++)
//           ui->ChatTable->setRowHidden(i,false);//为false就是显示
//    }
//    else{
//    //然后把所有行都隐藏
//    for(int i=0;i<ui->ChatTable->rowCount();i++)
//        ui->ChatTable->setRowHidden(i,true);//隐藏
//    //判断符合条件索引是不是空
//    if(!findItemsList.empty())//恢复对应的行
//      for(int i=0;i<findItemsList.count();i++)
//         ui->ChatTable->setRowHidden(findItemsList.at(i)->row(),false);//回复对应的行，也可以回复列
//    }
}

void AdminManagement::on_DeleteButton_2_clicked()
{
//    if(QMessageBox::Yes==QMessageBox::question(this,"再次确认","再次确认是否删除")){
//        int curRow=ui->ChatTable->currentRow();     //当前行号
//        ui->ChatTable->removeRow(curRow);           //删除当前行及其items
//    }
    //发送删除信息
}

void AdminManagement::on_ManageUserButton_clicked()
{
    //确保id和name都有item存在，否则会异常
//    int curRow=ui->ChatTable->currentRow();
//    UserManage * usermanage = new UserManage;
//    usermanage->init(ui->ChatTable->item(curRow,1)->text(),ui->ChatTable->item(curRow,0)->text());
//    usermanage->show();
}

void AdminManagement::on_ManageChatButton_clicked()
{
    //确保id和name都有item存在，否则会异常
//    int curRow=ui->ChatTable->currentRow();
//    MessageManage * messagemanage = new MessageManage;
//    messagemanage->init(ui->ChatTable->item(curRow,1)->text(),ui->ChatTable->item(curRow,0)->text());
//    messagemanage->show();
}
