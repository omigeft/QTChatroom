#include "messagemanage.h"
#include "ui_messagemanage.h"
#include <QMessageBox>
MessageManage::MessageManage(const QString &name, const QString &id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessageManage) {
    ui->setupUi(this);

    Roomname = name;
    RoomID = id;
    ui->ChatName->setText("聊天名称:"+name);
    ui->ChatID->setText("聊天ID:"+id);


    QSqlQuery query;
    query.exec(QString(
        "CREATE VIEW IF NOT EXISTS chat_message_%1 AS "
        "SELECT m.m_id, m.m_u_id, u.u_name, m.m_t, m.m_text FROM message m "
        "JOIN user u ON m.m_u_id = u.u_id "
        "WHERE m.m_c_id = %1 "
        "ORDER BY m.m_id ASC;")
        .arg(RoomID));
    if (query.lastError().isValid()) {
        qDebug() << query.lastError();
        return;
    }

    chatMessageTableModel = new QSqlTableModel(this);
    chatMessageTableModel->setTable(QString("chat_message_%1").arg(RoomID));
    chatMessageTableModel->select();
    chatMessageTableModel->setHeaderData(0, Qt::Horizontal, "消息ID");
    chatMessageTableModel->setHeaderData(1, Qt::Horizontal, "发送者用户ID");
    chatMessageTableModel->setHeaderData(2, Qt::Horizontal, "发送者用户名");
    chatMessageTableModel->setHeaderData(3, Qt::Horizontal, "发送时间");
    chatMessageTableModel->setHeaderData(4, Qt::Horizontal, "内容");

    ui->MessageTable->setModel(chatMessageTableModel);
    ui->MessageTable->setColumnWidth(4, 160);

    //伴随父窗口关闭
//    setAttribute(Qt::WA_QuitOnClose,false);
//    //不允许通过其他方式修改
//    ui->MessageTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    //行头自适应表格
//    ui->MessageTable->horizontalHeader()->setStretchLastSection(true);
//    //设置表格中每一行的表头
//    ui->MessageTable->setHorizontalHeaderLabels(QStringList() << "发送时间" << "用户ID"<< "用户名" << "消息内容");

//    //测试使用
//    insertMessageIitem(0,0,"C语言");
//    insertMessageIitem(0,1,"http://c.tian.net/c/");
//    insertMessageIitem(0,2,"已更新");
//    insertMessageIitem(0,3,"内容0");
//    insertMessageIitem(1,3,"内容1");
//    insertMessageIitem(2,3,"内容2");
//    insertMessageIitem(3,3,"内容3");
}

MessageManage::~MessageManage() {
    delete ui;
}

void MessageManage::on_FindUserButton_clicked() {
    QString userName = ui->ContentInput->text();
    chatMessageTableModel->setFilter(QString("u_name LIKE '%%1%'").arg(userName));
    chatMessageTableModel->select();
}

void MessageManage::on_FindContentButton_clicked() {
    QString content = ui->ContentInput->text();
    chatMessageTableModel->setFilter(QString("m_text LIKE '%%1%'").arg(content));
    chatMessageTableModel->select();
}

void MessageManage::on_DeleteButton_clicked() {
//    if(QMessageBox::Yes==QMessageBox::question(this,"再次确认","再次确认是否删除")){
//        int curRow=ui->MessageTable->currentRow();     //当前行号
//        ui->MessageTable->removeRow(curRow);           //删除当前行及其items
//    }
    //发送删除信息
}
