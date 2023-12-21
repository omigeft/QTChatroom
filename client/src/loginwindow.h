#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QMessageBox>
#include <QtNetwork>
#include "clientcore.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;
    ~LoginWindow();

private slots:
    void on_CloseButton_clicked();

    void on_RegisterButton_clicked();

    void on_LoginButton_clicked();

private:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    QPoint diff_pos;
    Ui::LoginWindow *ui;
    ClientCore *core;
};

#endif // LOGINWINDOW_H
