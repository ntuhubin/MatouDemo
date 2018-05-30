#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->label_title->setText("集装箱卡车违停报警系统");
    ui->label_Alarm->setText("报警信息");
}

Widget::~Widget()
{
    delete ui;
}
