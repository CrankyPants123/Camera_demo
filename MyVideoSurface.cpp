#include "MyVideoSurface.h"
#include "MainWindow.h"
#include <QtWidgets>
#include <qabstractvideosurface.h>
#include <qvideosurfaceformat.h>
#include <QVideoSurfaceFormat>

MyVideoSurface::MyVideoSurface(QWidget *widget, QObject *parent)
    : QAbstractVideoSurface(parent)
    , widget_(widget)
    , imageFormat_(QImage::Format_Invalid)
{

}

QList<QVideoFrame::PixelFormat> MyVideoSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
    if (handleType == QAbstractVideoBuffer::NoHandle) {
        return QList<QVideoFrame::PixelFormat>()<< QVideoFrame::Format_RGB32<< QVideoFrame::Format_ARGB32<< QVideoFrame::Format_ARGB32_Premultiplied<< QVideoFrame::Format_RGB565<< QVideoFrame::Format_RGB555;
    } else {
        return QList<QVideoFrame::PixelFormat>();
    }
}

bool MyVideoSurface::isFormatSupported(const QVideoSurfaceFormat & format) const
{
    return QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat()) != QImage::Format_Invalid && !format.frameSize().isEmpty() && format.handleType() == QAbstractVideoBuffer::NoHandle;
}

bool MyVideoSurface::start(const QVideoSurfaceFormat &format)
{
    const QImage::Format imageFormat_ = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
    const QSize size = format.frameSize();
    if (imageFormat_ != QImage::Format_Invalid && !size.isEmpty()) {
        this->imageFormat_ = imageFormat_;
        widget_->resize(size);
        QAbstractVideoSurface::start(format);
        widget_->updateGeometry();
        updateVideoRect();
        return true;
    }
    return false;
}

void MyVideoSurface::stop()
{
    currentFrame_ = QVideoFrame();
    targetRect_ = QRect();
    QAbstractVideoSurface::stop();
    widget_->update();
}

bool MyVideoSurface::present(const QVideoFrame &frame) //每一帧摄像头的数据，都会经过这里
{
    if (surfaceFormat().pixelFormat() != frame.pixelFormat() || surfaceFormat().frameSize() != frame.size()) {
        setError(IncorrectFormatError);
        stop();
        return false;
    }
    currentFrame_ = frame;
    widget_->repaint(targetRect_);
    return true;
}

QRect MyVideoSurface::videoRect() const
{
    return targetRect_;
}

void MyVideoSurface::updateVideoRect()
{
    QSize size = surfaceFormat().sizeHint();
    size.scale(widget_->size().boundedTo(size), Qt::KeepAspectRatio);
    targetRect_ = QRect(QPoint(0, 0), size);
    targetRect_.moveCenter(widget_->rect().center());
}

void MyVideoSurface::paint_nature(QPainter *painter)//绘制每一帧数据
{
    if (currentFrame_.map(QAbstractVideoBuffer::ReadOnly)) {
        //img就是转换的数据了
//        获取radiobutton状态 返回id值
//        static int i=1;
//        qDebug()<<"filter status:"<<i++;
//        灰度处理部分
//        qDebug()<<"nature";
        QImage img = QImage(currentFrame_.bits(),currentFrame_.width(),currentFrame_.height(),currentFrame_.bytesPerLine(),imageFormat_).mirrored(true,false).scaled(widget_->size());
        painter->drawImage(targetRect_, img, QRect(QPoint(0,0),img.size()));
        currentFrame_.unmap();
    }
}
void MyVideoSurface::paint_grey(QPainter *painter)
{
    if (currentFrame_.map(QAbstractVideoBuffer::ReadOnly)) {
//        灰度处理部分
        QImage img = QImage(currentFrame_.bits(),currentFrame_.width(),currentFrame_.height(),currentFrame_.bytesPerLine(),imageFormat_).mirrored(true,false).scaled(widget_->size());
        QImage * newImage = new QImage(img.width(), img.height(), QImage::Format_ARGB32);

//        QRgb * line;
        for(int y = 0; y<newImage->height(); y++){
            QRgb * line = (QRgb *)img.scanLine(y);

            for(int x = 0; x<newImage->width(); x++){
                int average = (qRed(line[x]) + qGreen(line[x]) + qRed(line[x]))/3;
                newImage->setPixel(x,y, qRgb(average, average, average));
            }
        }
        painter->drawImage(targetRect_, *newImage, QRect(QPoint(0,0),img.size()));
        currentFrame_.unmap();
    }
}
void MyVideoSurface::paint_warm(QPainter *painter)
{
    if (currentFrame_.map(QAbstractVideoBuffer::ReadOnly)) {
        QImage img = QImage(currentFrame_.bits(),currentFrame_.width(),currentFrame_.height(),currentFrame_.bytesPerLine(),imageFormat_).mirrored(true,false).scaled(widget_->size());

        QImage *newImage = new QImage(img.width(), img.height(), QImage::Format_ARGB32);

        QColor oldColor;
        int r,g,b;

        for(int x=0; x<newImage->width(); x++){
            for(int y=0; y<newImage->height(); y++){
                oldColor = QColor(img.pixel(x,y));

//                r = oldColor.red() + ;
//                为红绿通道增加的值,这里直接写死
                r = oldColor.red() + 50;
                g = oldColor.green() + 50;
                b = oldColor.blue();

                //we check if the new values are between 0 and 255
                r = qBound(0, r, 255);
                g = qBound(0, g, 255);

                newImage->setPixel(x,y, qRgb(r,g,b));
            }
        }

        painter->drawImage(targetRect_, *newImage, QRect(QPoint(0,0),img.size()));
        currentFrame_.unmap();
    }
}
