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
    ~MessageManage();

private:
    Ui::MessageManage *ui;
};

#endif // MESSAGEMANAGE_H
