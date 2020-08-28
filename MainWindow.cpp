#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QtWidgets>
#include <QVideoSurfaceFormat>
#include <QCameraViewfinderSettings>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
      Init();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::Init(){
//    申请相机资源，初始化参数
    ui->setupUi(this);
    camera_ = new QCamera;
    QCameraViewfinderSettings m_viewfinder;
    m_viewfinder.setResolution(640,480);
    m_viewfinder.setMinimumFrameRate(30.0);
    surface_ = new MyVideoSurface(this);
    camera_->setViewfinder(surface_);
    camera_->setViewfinderSettings(m_viewfinder);
    camera_->start();
    m_buttongroup = new QButtonGroup(this);
    m_buttongroup->addButton(ui->b_nature,1);
    m_buttongroup->addButton(ui->b_grey,2);
    m_buttongroup->addButton(ui->b_warm,3);
//    获取设备支持的分辨率
//    qDebug()<<camera_->supportedViewfinderResolutions();

    m_timer = new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(frame_count()));
    m_timer->start(1000);

}
QSize MainWindow::sizeHint() const
{
    return surface_->surfaceFormat().sizeHint();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if (surface_->isActive()) {
        const QRect videoRect = surface_->videoRect();
        if (!videoRect.contains(event->rect())) {
            QRegion region = event->region();
            region = region.subtracted(videoRect);
            QBrush brush = palette().background();

            painter.fillRect(region.boundingRect(), brush);
//            for (const QRect &rect : region){
//            for(auto& rect : region){
//                painter.fillRect(rect, brush);
//            }
        }
//        qDebug()<<m_buttongroup->checkedId();
        switch(m_buttongroup->checkedId()){
            case 1: surface_->paint_nature(&painter);break;//在主窗口绘制
//        case 1: qDebug()<<"case1"<<m_buttongroup->checkedId();break;
            case 2: surface_->paint_grey(&painter);break;//在主窗口绘制
//        case 2: qDebug()<<"case2"<<m_buttongroup->checkedId();break;
            case 3: surface_->paint_warm(&painter);break;//在主窗口绘制
        }

    } else {
        painter.fillRect(event->rect(), palette().background());
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    surface_->updateVideoRect();
}

void MainWindow::frame_count(){
    qDebug()<<"frame rate now:"<<m_framerate;
}
