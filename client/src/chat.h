#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include <QStringList>

namespace Ui {
class Chat;
}

class Chat : public QWidget
{
    Q_OBJECT

public:
    explicit Chat(QWidget *parent = nullptr);
    QString UserName;
    QString ChatName;
    void ChatInit(QString username,QString chatname);
    ~Chat();

private slots:
    void on_CloseButton_clicked();

    void on_SendButton_clicked();

    void AddMessage(QString name,QString Message,QString time);

    void updateChatHistory();

private:
    Ui::Chat *ui;
    QStringList userlist;
};

#endif // CHAT_H
