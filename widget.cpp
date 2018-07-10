#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->label_title->setText("集装箱卡车违停报警系统");
    ui->label_Alarm->setText("报警信息");
    ui->btnStart->setText("启动监控");
    ui->btnExit->setText("停止监控");
    connect(ui->btnStart, &QPushButton::clicked, this, &Widget::MonitorStart);
    connect(ui->btnExit, &QPushButton::clicked, this, &Widget::MonitorExit);
    play_thread = new PlayImgThread();
    connect(play_thread, &PlayImgThread::message, this, &Widget::recvImg);
}

Widget::~Widget()
{
    delete ui;
    delete play_thread;
}
void Widget::recvImg(QImage img)
{
    int w = ui->label_Camera->width();
    int h = ui->label_Camera->height();
    QPixmap mp = QPixmap::fromImage(img);
    QPixmap fitpixmap = mp.scaled(w, h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
    ui->label_Camera->setPixmap(fitpixmap);
}
void Widget::MonitorStart()
{
    play_thread->start();
}
void Widget::MonitorExit()
{

}
