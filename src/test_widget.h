#ifndef TEST_WIDGET_H
#define TEST_WIDGET_H

#include <QWidget>
#include "algo.h"

extern algo antalgo;

namespace Ui {
class test_widget;
}

class test_widget : public QWidget
{
    Q_OBJECT

public:
    explicit test_widget(QWidget *parent = nullptr);
    ~test_widget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

signals:
    void button_clicked();
    void button_2_clicked();

public:
    Ui::test_widget *ui;
};

#endif // TEST_WIDGET_H
