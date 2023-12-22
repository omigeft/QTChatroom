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
    ui(new Ui::AdminManagement) {
    ui->setupUi(this);
    // 获取核心实例
    core = &ServerCore::getInstance();
    //:/icon/manage.png
    //设置该聊天窗口的标题-----管理员名
    this->setWindowTitle("当前管理员：" + core->adminUserName);

    ui->UserTable->setModel(core->userTableModel);
    ui->ChatTable->setModel(core->chatTableModel);

    ui->UserTable->setColumnWidth(3, 160);
    ui->UserTable->setColumnWidth(4, 160);
    ui->ChatTable->setColumnWidth(3, 160);
    ui->ChatTable->setColumnWidth(4, 160);

    ui->DeleteUserButton->setEnabled(false);
    ui->DeleteChatButton->setEnabled(false);
    ui->ManageUserButton->setEnabled(false);
    ui->ManageChatButton->setEnabled(false);

    // 如果ui->UserTable至少选中了一行，则激活ui->DeleteUserButton
    connect(ui->UserTable->selectionModel(), &QItemSelectionModel::selectionChanged, [=](){
        ui->DeleteUserButton->setEnabled(!ui->UserTable->selectionModel()->selectedRows().isEmpty());
    });

    // 如果ui->ChatTable至少选中了一行，则激活ui->DeleteChatButton
    connect(ui->ChatTable->selectionModel(), &QItemSelectionModel::selectionChanged, [=](){
        ui->DeleteChatButton->setEnabled(!ui->ChatTable->selectionModel()->selectedRows().isEmpty());
    });

    // 如果ui->ChatTable选中且仅选中了一行，则激活ui->ManageUserButton和ui->ManageChatButton
    connect(ui->ChatTable->selectionModel(), &QItemSelectionModel::selectionChanged, [=](){
        ui->ManageUserButton->setEnabled(ui->ChatTable->selectionModel()->selectedRows().count() == 1);
    });
    connect(ui->ChatTable->selectionModel(), &QItemSelectionModel::selectionChanged, [=](){
        ui->ManageChatButton->setEnabled(ui->ChatTable->selectionModel()->selectedRows().count() == 1);
    });

    connect(core->userTableModel, &QSqlTableModel::dataChanged, this, &AdminManagement::onUserDataChanged);
    connect(core->chatTableModel, &QSqlTableModel::dataChanged, this, &AdminManagement::onChatDataChanged);
}

AdminManagement::~AdminManagement() {
    delete ui;
}

void AdminManagement::onUserDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles) {
    core->userTableModel->select();
}

void AdminManagement::onChatDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles) {
    core->chatTableModel->select();
}

void AdminManagement::on_FindUserButton_clicked() {
    QString userName = ui->UserInput->text();
    core->userTableModel->setFilter(QString("u_name LIKE '%%1%'").arg(userName));
    core->userTableModel->select();
}

void AdminManagement::on_FindChatButton_clicked() {
    QString chatName = ui->ChatInput->text();
    core->chatTableModel->setFilter(QString("c_name LIKE '%%1%'").arg(chatName));
    core->chatTableModel->select();
}

void AdminManagement::on_NewUserButton_clicked() {
    core->registerAccount(
        QString("User_%1").arg(core->maxUserNumber + 1),
        QString("Password_%1").arg(core->maxUserNumber + 1),
        "user");
}

void AdminManagement::on_NewChatButton_clicked() {
    core->createChatroom(
        QString("Chat_%1").arg(core->maxChatroomNumber + 1),
        core->adminUserName);
}

void AdminManagement::on_DeleteUserButton_clicked() {
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
        query.prepare("UPDATE user SET sd_t = :time WHERE u_id = :id");
        query.bindValue(":time", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        query.bindValue(":id", userID);
        query.exec();
        if (query.lastError().isValid()) {
            qDebug() << query.lastError();
            query.exec("ROLLBACK");
            core->userTableModel->select();
            return;
        }
        if (userID == core->adminUserID) {
            QMessageBox::critical(this, "错误", "不能删除管理员账户");
            query.exec("ROLLBACK");
            core->userTableModel->select();
            return;
        }
    }
    // 事务提交
    query.exec("COMMIT");
    // 刷新用户表
    core->userTableModel->select();
}

void AdminManagement::on_DeleteChatButton_clicked() {
    QItemSelectionModel *selectionModel = ui->ChatTable->selectionModel();
    QModelIndexList selectedRows = selectionModel->selectedRows();
    QModelIndex index_0;
    int chatID;
    QSqlQuery query;
    // 事务开始，保证原子性
    query.exec("BEGIN");
    for (const QModelIndex &index : selectedRows) {
        index_0 = ui->ChatTable->model()->index(index.row(), 0);
        chatID = ui->ChatTable->model()->data(index_0).toInt();
        query.prepare("UPDATE chatroom SET ds_t = :time WHERE c_id = :id");
        query.bindValue(":time", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        query.bindValue(":id", chatID);
        query.exec();
        if (query.lastError().isValid()) {
            qDebug() << query.lastError();
            query.exec("ROLLBACK");
            core->chatTableModel->select();
            return;
        }
    }
    // 事务提交
    query.exec("COMMIT");
    // 刷新聊天室表
    core->chatTableModel->select();
}

void AdminManagement::on_ManageUserButton_clicked() {
    QItemSelectionModel *selectionModel = ui->ChatTable->selectionModel();
    QModelIndexList selectedRows = selectionModel->selectedRows();
    int rowIndex = selectedRows.first().row();
    QModelIndex index_0 = ui->ChatTable->model()->index(rowIndex, 0);
    QString chatid = ui->ChatTable->model()->data(index_0).toString();
    QModelIndex index_1 = ui->ChatTable->model()->index(rowIndex, 1);
    QString chatname = ui->ChatTable->model()->data(index_1).toString();
    UserManage * usermanage = new UserManage(chatname, chatid);
    usermanage->show();
}

void AdminManagement::on_ManageChatButton_clicked() {
    QItemSelectionModel *selectionModel = ui->ChatTable->selectionModel();
    QModelIndexList selectedRows = selectionModel->selectedRows();
    int rowIndex = selectedRows.first().row();
    QModelIndex index_0 = ui->ChatTable->model()->index(rowIndex, 0);
    QString chatid = ui->ChatTable->model()->data(index_0).toString();
    QModelIndex index_1 = ui->ChatTable->model()->index(rowIndex, 1);
    QString chatname = ui->ChatTable->model()->data(index_1).toString();
    MessageManage * messagemanage = new MessageManage(chatname, chatid);
    messagemanage->show();
}
