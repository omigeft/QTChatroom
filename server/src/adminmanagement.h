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
    void on_ChangeButton_clicked();

    void on_DeleteButton_clicked();

    void on_FindButton_clicked();

    void on_FindButton_2_clicked();

    void on_DeleteButton_2_clicked();

    void on_ManageUserButton_clicked();

    void on_ManageChatButton_clicked();

private:
    Ui::AdminManagement *ui;
    ServerCore *core;
    void insertUserIitem(int row,int column,QString content);
    void insertChatIitem(int row,int column,QString content);
};

#endif // ADMINMANAGEMENT_H
