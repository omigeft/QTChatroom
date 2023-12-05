#ifndef CHATLIST_H
#define CHATLIST_H

#include <QWidget>

namespace Ui {
class ChatList;
}

class ChatList : public QWidget
{
    Q_OBJECT

public:
    explicit ChatList(QWidget *parent = nullptr);
    ~ChatList();

private:
    Ui::ChatList *ui;
};

#endif // CHATLIST_H
