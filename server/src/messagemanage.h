#ifndef MESSAGEMANAGE_H
#define MESSAGEMANAGE_H

#include <QWidget>
#include <QtSql>

namespace Ui {
class MessageManage;
}

class MessageManage : public QWidget
{
    Q_OBJECT

public:
    explicit MessageManage(const QString &name, const QString &id, QWidget *parent = nullptr);
    ~MessageManage();

private slots:
    void on_DeleteButton_clicked();

    void on_FindButton_clicked();

private:
    void insertMessageIitem(int row, int column, const QString &content);

    void refreshChatMessageTable();

private:
    QString Roomname;
    QString RoomID;
    Ui::MessageManage *ui;
    QSqlTableModel* chatMessageTableModel;
};

#endif // MESSAGEMANAGE_H
