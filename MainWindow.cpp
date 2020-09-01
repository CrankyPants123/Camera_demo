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
    camera_->setCaptureMode(QCamera::CaptureStillImage);
    camera_->setViewfinder(surface_);
    camera_->setViewfinderSettings(m_viewfinder);
    camera_->start();
    m_buttongroup = new QButtonGroup(this);
    m_buttongroup->addButton(ui->b_nature,1);
    m_buttongroup->addButton(ui->b_grey,2);
    m_buttongroup->addButton(ui->b_warm,3);
    m_buttongroup->addButton(ui->b_cold,4);
    m_buttongroup->addButton(ui->b_saturation,5);
    m_buttongroup->addButton(ui->b_metal,6);
    connect(ui->action1920_x_1080,SIGNAL(triggered()),this,SLOT(switch_1080()));
    connect(ui->action1280_x_800,SIGNAL(triggered()),this,SLOT(switch_800()));
    connect(ui->action1280_x_720,SIGNAL(triggered()),this,SLOT(switch_720()));
    connect(ui->action640_x_480,SIGNAL(triggered()),this,SLOT(switch_480()));
    connect(ui->action352_x_288,SIGNAL(triggered()),this,SLOT(switch_288()));
    connect(ui->b_cheese,SIGNAL(clicked()),this,SLOT(save_image()));
    connect(ui->b_cheese,SIGNAL(clicked()),this,SLOT(image_display()));

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
       m_framerate++;
        switch(m_buttongroup->checkedId()){
            case 1: surface_->paint_nature(&painter);break;//在主窗口绘制
            case 2: surface_->paint_grey(&painter);break;
            case 3: surface_->paint_warm(&painter);break;
            case 4: surface_->paint_cold(&painter);break;
            case 5: surface_->paint_nature(&painter);break;
            case 6: surface_->paint_nature(&painter);break;
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
    m_framerate = 0;
}

void MainWindow::switch_1080(){
    QCameraViewfinderSettings m_viewfinder_2;
    m_viewfinder_2.setResolution(1920,1080);
    camera_->setViewfinderSettings(m_viewfinder_2);
}
void MainWindow::switch_800(){
    QCameraViewfinderSettings m_viewfinder_2;
    m_viewfinder_2.setResolution(1280,800);
    camera_->setViewfinderSettings(m_viewfinder_2);
}

void MainWindow::switch_720(){
    QCameraViewfinderSettings m_viewfinder_2;
    m_viewfinder_2.setResolution(1280,720);
//    m_viewfinder_2.setMinimumFrameRate(30.0);
    camera_->setViewfinderSettings(m_viewfinder_2);
}
void MainWindow::switch_480(){
    QCameraViewfinderSettings m_viewfinder_2;
    m_viewfinder_2.setResolution(640,480);
    camera_->setViewfinderSettings(m_viewfinder_2);
}

void MainWindow::switch_288(){
    QCameraViewfinderSettings m_viewfinder_2;
    m_viewfinder_2.setResolution(352,288);
    camera_->setViewfinderSettings(m_viewfinder_2);
}
void MainWindow::save_image(){
    qDebug()<<"image save";
}
void MainWindow::image_display(){
    qDebug()<<"imagedisplay_reserve";
}
