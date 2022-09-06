#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    cam = nullptr;//先让指针指向空
    timer = new QTimer(this);//创建一个定时器对象

    ui->label->installEventFilter(this);//安装label的事件过滤器

    connect(timer, &QTimer::timeout,
            [=]()
    {
        frame = cvQueryFrame(cam);// 从摄像头中抓取并返回每一帧
        // 将抓取到的帧，转换为QImage格式 QImage::Format_RGB888不同的摄像头用不同的格式
        QImage image = QImage((const uchar*)frame->imageData, frame->width, frame->height, QImage::Format_RGB888).rgbSwapped();
        ui->label->setPixmap(QPixmap::fromImage(image));  // 将图片显示到label上
    }
    );//将定时器和图像显示关联
}

Widget::~Widget()
{
    delete ui;
}

bool Widget::eventFilter(QObject *obj, QEvent *event)//重写事件
{
    if (event->type() == QEvent::MouseButtonDblClick)//是鼠标双击进
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);//数据类型转换
        if(mouseEvent->button() == Qt::LeftButton)//是鼠标左击进
        {
            if(obj == ui->label)//是label子控件进
            {
                if(ui->label->isFullScreen())//子控件全屏进
                {
                    //子控件恢复原来大小
                    ui->label->setWindowFlags (Qt::SubWindow);
                    ui->label->showNormal ();
                }
                else//子控件未全屏进
                {
                    //将子控件设置全屏
                    ui->label->setWindowFlags (Qt::Window);
                    ui->label->showFullScreen ();
                }
                return true;
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}

void Widget::on_stop_clicked()//停止按钮槽函数
{
    if(timer->isActive() == true)//定时器打开进入
    {
        timer->stop(); // 停止定时器
        cvReleaseCapture(&cam);//释放内存；
        ui->label->setText("图像传输结束\n\n继续显示请点击开始按钮");//显示文字
    }
}

void Widget::on_start_clicked()//打开按钮槽函数
{
    if(timer->isActive() == false)//定时器关闭进入
    {
        cam = cvCreateCameraCapture(0);//打开摄像头，从摄像头中获取视频
        timer->start(40);//打开定时器 单位ms
    }
}
