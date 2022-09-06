#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QObject>
#include <QImage>
#include <QTimer>
#include <QMouseEvent>

#include <highgui.h>  //包含opencv库头文件
#include <cv.h>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_stop_clicked();//开始按钮槽函数

    void on_start_clicked();//停止按钮槽函数

protected:
    bool eventFilter(QObject *obj, QEvent *event);//重写eventFilter事件

private:
    Ui::Widget *ui;
    QTimer    *timer;//定时器
    CvCapture *cam;// 视频获取结构 用来作为视频获取函数的一个参数
    IplImage  *frame;//申请IplImage类型指针 就是申请内存空间来存放每一帧图像
};

#endif // WIDGET_H
