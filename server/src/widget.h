#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "servercore.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_CloseButton_clicked();

    void on_LoginButton_clicked();

private:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    QPoint diff_pos;
    Ui::Widget *ui;
    ServerCore *core;
};
#endif // WIDGET_H
