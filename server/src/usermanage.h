#ifndef USERMANAGE_H
#define USERMANAGE_H

#include <QWidget>

namespace Ui {
class UserManage;
}

class UserManage : public QWidget
{
    Q_OBJECT

public:
    explicit UserManage(QWidget *parent = nullptr);
    void init(QString name,QString id);
    ~UserManage();

private slots:
    void on_ChangeButton_clicked();

    void on_DeleteButton_clicked();

    void on_FindButton_clicked();

private:
    QString Roomname;
    QString RoomID;
    Ui::UserManage *ui;
    void insertUserIitem(int row,int column,QString content);
};

#endif // USERMANAGE_H
