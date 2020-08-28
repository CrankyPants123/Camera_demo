#ifndef MYVIDEOSURFACE_H
#define MYVIDEOSURFACE_H

#include <QAbstractVideoSurface>
#include <QImage>
#include <QRect>
#include<QButtonGroup>
#include <QVideoFrame>
#include<QRadioButton>

class MyVideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT

public:
    MyVideoSurface(QWidget *widget, QObject *parent = 0);
//    void Init();
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const override;
    bool isFormatSupported(const QVideoSurfaceFormat &format) const override;
    bool start(const QVideoSurfaceFormat &format) override;
    bool present(const QVideoFrame &frame) override;
    void stop() override;
    QRect videoRect() const;
    void updateVideoRect();
    void paint_nature(QPainter *painter);
    void paint_grey(QPainter *painter);
    void paint_warm(QPainter *painter);


private:
    QWidget      * widget_;
    QImage::Format imageFormat_;
    QRect          targetRect_;
    QSize          imageSize_;
    QVideoFrame    currentFrame_;

//    QButtonGroup   * m_buttongroup;
//    QRadioButton   * b_nature;
//    QRadioButton   * b_grey;



};

#endif
