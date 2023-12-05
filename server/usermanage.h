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
    ~UserManage();

private:
    Ui::UserManage *ui;
};

#endif // USERMANAGE_H
