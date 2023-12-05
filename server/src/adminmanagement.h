#ifndef ADMINMANAGEMENT_H
#define ADMINMANAGEMENT_H

#include <QWidget>

namespace Ui {
class AdminManagement;
}

class AdminManagement : public QWidget
{
    Q_OBJECT

public:
    explicit AdminManagement(QWidget *parent = nullptr);
    ~AdminManagement();

private:
    Ui::AdminManagement *ui;
};

#endif // ADMINMANAGEMENT_H
