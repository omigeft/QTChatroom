#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include <QStringList>
#include <QString>
#include <QRandomGenerator>
#include <QDebug>
#include <QDateTime>
#include <QTimer>
#include "ui_chat.h"
#include "clientcore.h"

namespace Ui {
class Chat;
}

class Chat : public QWidget
{
    Q_OBJECT

public:
    explicit Chat(const QString &chatName, QWidget *parent = nullptr);
    ~Chat();

    void refreshChat();

private slots:
    void on_CloseButton_clicked();

    void on_SendButton_clicked();

private:
    Ui::Chat *ui;
    ClientCore *core;

    QString currentChatName;
    QStringList userList;
    QStringList chatHistory;

    int latestMessageID;
    QTimer *timer;
};

#endif // CHAT_H
