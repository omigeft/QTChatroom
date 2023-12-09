#include "usermanage.h"
#include "ui_usermanage.h"
#include <QMessageBox>
UserManage::UserManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserManage)
{
    ui->setupUi(this);
    //伴随父窗口关闭
//    setAttribute(Qt::WA_QuitOnClose,false);
//    //不允许通过其他方式修改
//    ui->UserTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    //行头自适应表格
//    ui->UserTable->horizontalHeader()->setStretchLastSection(true);
//    //设置表格中每一行的表头
//    ui->UserTable->setHorizontalHeaderLabels(QStringList() << "ID" << "用户名" << "密码"<<"注册时间");

//    //测试使用
//    insertUserIitem(0,0,"C语言");
//    insertUserIitem(0,1,"http://c.tian.net/c/");
//    insertUserIitem(0,2,"已更新");
//    insertUserIitem(0,3,"更新");
}

UserManage::~UserManage()
{
    delete ui;
}
void UserManage::init(QString name,QString id)
{
    Roomname = name;
    RoomID = id;
    ui->ChatName->setText("聊天名称:"+name);
    ui->ChatID->setText("聊天ID:"+id);
}

void UserManage::on_ChangeButton_clicked()
{
//    if(QMessageBox::Yes==QMessageBox::question(this,"再次确认","再次确认是否修改")){
//    ui->UserTable->currentItem()->setText(ui->ContentInput->text());
//    ui->ContentInput->text().clear();
//    }
    //发送修改信息
}

void UserManage::on_DeleteButton_clicked()
{
//    if(QMessageBox::Yes==QMessageBox::question(this,"再次确认","再次确认是否删除")){
//        int curRow=ui->UserTable->currentRow();     //当前行号
//        ui->UserTable->removeRow(curRow);           //删除当前行及其items
//    }
    //发送删除信息
}

void UserManage::on_FindButton_clicked()
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
void UserManage::insertUserIitem(int row,int column,QString content)
{
//    ui->UserTable->setItem(row,column,new QTableWidgetItem(content));
}
