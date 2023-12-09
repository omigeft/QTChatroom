#ifndef CHATLIST_H
#define CHATLIST_H

#include <QWidget>
#include <QStringList>
#include <QRegularExpression>

namespace Ui {
class ChatList;
}

class ChatList : public QWidget
{
    Q_OBJECT

public:
    QString UserName;
    explicit ChatList(QWidget *parent = nullptr);
    void initChatList();
    ~ChatList();

private slots:
    void on_OpenChatButton_clicked();

    void on_JoinButton_clicked();

    void on_NewChatButton_clicked();

    void on_SortChatButton_clicked();

private:
    QStringList Joinlist;
    QStringList UJoinlist;
    QStringList selectlist;
    Ui::ChatList *ui;
};

#endif // CHATLIST_H
