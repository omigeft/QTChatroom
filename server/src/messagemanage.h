#ifndef MESSAGEMANAGE_H
#define MESSAGEMANAGE_H

#include <QWidget>

namespace Ui {
class MessageManage;
}

class MessageManage : public QWidget
{
    Q_OBJECT

public:
    explicit MessageManage(QWidget *parent = nullptr);
    void init(QString name,QString id);
    ~MessageManage();

private slots:
    void on_DeleteButton_clicked();

    void on_FindButton_clicked();

private:
    QString Roomname;
    QString RoomID;
    Ui::MessageManage *ui;
    void insertMessageIitem(int row,int column,QString content);
};

#endif // MESSAGEMANAGE_H
