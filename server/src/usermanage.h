#ifndef USERMANAGE_H
#define USERMANAGE_H

#include <QWidget>
#include <QtSql>

namespace Ui {
class UserManage;
}

class UserManage : public QWidget
{
    Q_OBJECT

public:
    explicit UserManage(const QString &name, const QString &id, QWidget *parent = nullptr);
    ~UserManage();

private slots:
    void on_ChangeButton_clicked();

    void on_DeleteButton_clicked();

    void on_FindButton_clicked();

private:
    void insertUserIitem(int row, int column, const QString &content);

private:
    QString Roomname;
    QString RoomID;
    Ui::UserManage *ui;
    QSqlTableModel* chatUserTableModel;
};

#endif // USERMANAGE_H
