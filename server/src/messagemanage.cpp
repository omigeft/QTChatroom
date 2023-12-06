#include "messagemanage.h"
#include "ui_messagemanage.h"
#include <QMessageBox>
MessageManage::MessageManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessageManage)
{
    ui->setupUi(this);
    //伴随父窗口关闭
    setAttribute(Qt::WA_QuitOnClose,false);
    //不允许通过其他方式修改
    ui->MessageTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //行头自适应表格
    ui->MessageTable->horizontalHeader()->setStretchLastSection(true);
    //设置表格中每一行的表头
    ui->MessageTable->setHorizontalHeaderLabels(QStringList() << "发送时间" << "用户ID"<< "用户名" << "消息内容");

    //测试使用
    insertMessageIitem(0,0,"C语言");
    insertMessageIitem(0,1,"http://c.tian.net/c/");
    insertMessageIitem(0,2,"已更新");
    insertMessageIitem(0,3,"内容0");
    insertMessageIitem(1,3,"内容1");
    insertMessageIitem(2,3,"内容2");
    insertMessageIitem(3,3,"内容3");
}

MessageManage::~MessageManage()
{
    delete ui;
}
void MessageManage::init(QString name,QString id)
{
    Roomname = name;
    RoomID = id;
    ui->ChatName->setText("聊天名称:"+name);
    ui->ChatID->setText("聊天ID:"+id);
}
void MessageManage::insertMessageIitem(int row,int column,QString content)
{
    ui->MessageTable->setItem(row,column,new QTableWidgetItem(content));
}

void MessageManage::on_DeleteButton_clicked()
{
    if(QMessageBox::Yes==QMessageBox::question(this,"再次确认","再次确认是否删除")){
        int curRow=ui->MessageTable->currentRow();     //当前行号
        ui->MessageTable->removeRow(curRow);           //删除当前行及其items
    }
    //发送删除信息
}

void MessageManage::on_FindButton_clicked()
{
    QList<QTableWidgetItem *> findItemsList = ui->MessageTable->findItems(ui->ContentInput->text(),Qt::MatchContains);
    if (ui->ContentInput->text()==""){//判断是否是空，如果是空就显示所有行
       for(int i=0;i<ui->MessageTable->rowCount();i++)
           ui->MessageTable->setRowHidden(i,false);//为false就是显示
    }
    else{
    //然后把所有行都隐藏
    for(int i=0;i<ui->MessageTable->rowCount();i++)
        ui->MessageTable->setRowHidden(i,true);//隐藏
    //判断符合条件索引是不是空
    if(!findItemsList.empty())//恢复对应的行
      for(int i=0;i<findItemsList.count();i++)
         ui->MessageTable->setRowHidden(findItemsList.at(i)->row(),false);//回复对应的行，也可以回复列
    }
}
