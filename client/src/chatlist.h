#ifndef CHATLIST_H
#define CHATLIST_H

#include <QWidget>
#include <QStringList>
#include <QRegularExpression>
#include <QStringList>
#include <QDebug>
#include <QMessageBox>
#include "ui_chatlist.h"
#include "chat.h"
#include "clientcore.h"

namespace Ui {
class ChatList;
}

class ChatList : public QWidget
{
    Q_OBJECT

public:
    explicit ChatList(QWidget *parent = nullptr);
    ~ChatList();

private slots:
    void refreshChatList();

    void on_OpenChatButton_clicked();

    void on_JoinButton_clicked();

    void on_NewChatButton_clicked();

    void on_SortChatButton_clicked();

private:
    Ui::ChatList *ui;
    ClientCore *core;
};

#endif // CHATLIST_H
