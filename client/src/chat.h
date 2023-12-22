#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include <QStringList>
#include <QString>
#include <QRandomGenerator>
#include <QDebug>
#include <QMouseEvent>
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

private slots:
    void refreshUserList();

    void refreshChat();

    void on_CloseButton_clicked();

    void on_SendButton_clicked();

private:
    Ui::Chat *ui;
    ClientCore *core;

    QString currentChatName;
    QStringList userList;
    QStringList chatHistory;
    QList<int> chatMessageID;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    QPoint diff_pos;

    int latestMessageID;
    QString lastTime;
    QTimer *timer;
};

#endif // CHAT_H
