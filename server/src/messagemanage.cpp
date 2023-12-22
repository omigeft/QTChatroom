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
        "SELECT m.m_id, m.m_u_id, u.u_name, m.m_t, m.m_db_t, m.m_text FROM message m "
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
    chatMessageTableModel->setHeaderData(4, Qt::Horizontal, "撤回时间");
    chatMessageTableModel->setHeaderData(5, Qt::Horizontal, "内容");

    ui->MessageTable->setModel(chatMessageTableModel);
    ui->MessageTable->setColumnWidth(4, 160);
    ui->MessageTable->setColumnWidth(5, 160);

    ui->DeleteButton->setEnabled(false);

    // 如果ui->MessageTable至少选中了一行，则激活ui->DeleteButton
    connect(ui->MessageTable->selectionModel(), &QItemSelectionModel::selectionChanged, [=](){
        ui->DeleteButton->setEnabled(!ui->MessageTable->selectionModel()->selectedRows().isEmpty());
    });
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
    QItemSelectionModel *selectionModel = ui->MessageTable->selectionModel();
    QModelIndexList selectedRows = selectionModel->selectedRows();
    QModelIndex index_0;
    int messageID;
    QSqlQuery query;
    // 事务开始，保证原子性
    query.exec("BEGIN");
    // 删除所有选中的行的消息
    for (const QModelIndex &index : selectedRows) {
        index_0 = ui->MessageTable->model()->index(index.row(), 0);
        messageID = ui->MessageTable->model()->data(index_0).toInt();
        query.prepare("UPDATE message SET m_db_t = :drawback_time WHERE m_id = :message_id AND m_db_t IS NULL;");
        query.bindValue(":drawback_time", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        query.bindValue(":message_id", messageID);
        query.exec();
        if (query.lastError().isValid()) {
            qDebug() << query.lastError();
            query.exec("ROLLBACK");
            chatMessageTableModel->select();
            return;
        }
    }
    // 事务提交
    query.exec("COMMIT");
    // 刷新用户表
    chatMessageTableModel->select();
}
