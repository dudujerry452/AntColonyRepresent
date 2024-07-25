#include "test_widget.h"
#include "ui_test_widget.h"
#include "algo.h"

extern algo antalgo;

test_widget::test_widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::test_widget)
{
    ui->setupUi(this);
    QMetaObject::connectSlotsByName(this);
    ui->lineEdit_4->setText("1");
    ui->lineEdit_3->setText("6");
    ui->lineEdit_5->setText("1");
}

test_widget::~test_widget()
{
    delete ui;
}

void test_widget::on_pushButton_clicked()
{
    button_clicked();
    antalgo.m = ui->lineEdit_2->text().toInt();
}


void test_widget::on_pushButton_2_clicked()
{
    button_2_clicked();
}

