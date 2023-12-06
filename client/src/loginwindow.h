#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QMessageBox>
#include <QtNetwork>
#include "publicdata.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void on_CloseButton_clicked();

    void on_RegisterButton_clicked();

    void on_LoginButton_clicked();

private:
    Ui::LoginWindow *ui;
    PublicData *pd;
};

#endif // LOGINWINDOW_H
