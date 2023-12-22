#ifndef ADMINMANAGEMENT_H
#define ADMINMANAGEMENT_H

#include <QWidget>
#include "servercore.h"

namespace Ui {
class AdminManagement;
}

class AdminManagement : public QWidget
{
    Q_OBJECT

public:
    explicit AdminManagement(QWidget *parent = nullptr);
    ~AdminManagement();

private slots:
    void onUserDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);
    void onChatDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);
    void on_FindUserButton_clicked();
    void on_FindChatButton_clicked();
    void on_NewUserButton_clicked();
    void on_NewChatButton_clicked();
    void on_DeleteUserButton_clicked();
    void on_DeleteChatButton_clicked();
    void on_ManageUserButton_clicked();
    void on_ManageChatButton_clicked();

private:
    Ui::AdminManagement *ui;
    ServerCore *core;
    void insertUserIitem(int row,int column,QString content);
    void insertChatIitem(int row,int column,QString content);
};

#endif // ADMINMANAGEMENT_H
