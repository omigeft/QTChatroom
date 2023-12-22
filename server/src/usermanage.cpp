#include "usermanage.h"
#include "ui_usermanage.h"
#include <QMessageBox>
UserManage::UserManage(const QString &name, const QString &id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserManage) {
    ui->setupUi(this);

    Roomname = name;
    RoomID = id;
    ui->ChatName->setText("聊天名称:"+name);
    ui->ChatID->setText("聊天ID:"+id);

    QSqlQuery query;
    query.exec(QString(
        "CREATE VIEW IF NOT EXISTS chat_user_%1 AS "
        "SELECT u.u_id, u.u_name, uc.j_t, uc.q_t, uc.role FROM user_chatroom uc "
        "JOIN user u ON uc.u_id = u.u_id "
        "WHERE uc.c_id = %1 "
        "ORDER BY u.u_id ASC;"
        ).arg(RoomID));
    if (query.lastError().isValid()) {
        qDebug() << query.lastError();
        return;
    }

    chatUserTableModel = new QSqlTableModel(this);
    chatUserTableModel->setTable(QString("chat_user_%1").arg(RoomID));
    chatUserTableModel->select();
    chatUserTableModel->setHeaderData(0, Qt::Horizontal, "用户ID");
    chatUserTableModel->setHeaderData(1, Qt::Horizontal, "用户名");
    chatUserTableModel->setHeaderData(2, Qt::Horizontal, "加入时间");
    chatUserTableModel->setHeaderData(3, Qt::Horizontal, "退出时间");
    chatUserTableModel->setHeaderData(4, Qt::Horizontal, "群聊中角色");

    ui->UserTable->setModel(chatUserTableModel);
    ui->UserTable->setColumnWidth(3, 160);
    ui->UserTable->setColumnWidth(4, 160);

    ui->DeleteButton->setEnabled(false);

    // 如果ui->UserTable至少选中了一行，则激活ui->DeleteButton
    connect(ui->UserTable->selectionModel(), &QItemSelectionModel::selectionChanged, [=](){
        ui->DeleteButton->setEnabled(!ui->UserTable->selectionModel()->selectedRows().isEmpty());
    });
}

UserManage::~UserManage() {
    delete ui;
}

void UserManage::on_FindButton_clicked() {
    QString userName = ui->ContentInput->text();
    chatUserTableModel->setFilter(QString("u_name LIKE '%%1%'").arg(userName));
    chatUserTableModel->select();
}

void UserManage::on_DeleteButton_clicked() {
    QItemSelectionModel *selectionModel = ui->UserTable->selectionModel();
    QModelIndexList selectedRows = selectionModel->selectedRows();
    QModelIndex index_0;
    int userID;
    QSqlQuery query;
    // 事务开始，保证原子性
    query.exec("BEGIN");
    // 删除所有选中的行的用户
    for (const QModelIndex &index : selectedRows) {
        index_0 = ui->UserTable->model()->index(index.row(), 0);
        userID = ui->UserTable->model()->data(index_0).toInt();
        query.prepare("UPDATE user_chatroom SET q_t = :quit_time WHERE u_id = :user_id AND c_id = :chatroom_id AND q_t IS NULL;");
        query.bindValue(":quit_time", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        query.bindValue(":user_id", userID);
        query.bindValue(":chatroom_id", RoomID);
        query.exec();
        if (query.lastError().isValid()) {
            qDebug() << query.lastError();
            query.exec("ROLLBACK");
            chatUserTableModel->select();
            return;
        }
    }
    // 事务提交
    query.exec("COMMIT");
    // 刷新用户表
    chatUserTableModel->select();
}
