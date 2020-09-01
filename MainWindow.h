#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCamera>
#include "MyVideoSurface.h"
#include<QRadioButton>
#include<QButtonGroup>
#include<QCameraImageCapture>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::MainWindow * ui;
    MyVideoSurface * surface_;
    QCamera        * camera_;
    QButtonGroup   * m_buttongroup;
    QRadioButton   * b_nature;
    QRadioButton   * b_grey;
    QCameraImageCapture * imageCapture;
    void Init();
    int m_framerate = 0;
    QTimer         * m_timer;

private slots:
    void frame_count();
    void switch_1080();
    void switch_800();
    void switch_720();
    void switch_480();
    void switch_288();
    void save_image();
    void image_display();
};

#endif // MAINWINDOW_H
