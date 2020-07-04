#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointF>
#include <QPixmap>
#include <QPen>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_timeLimitCheck_stateChanged(int state);
    void on_startButton_clicked();

private:
    Ui::MainWindow *ui;
    QPointF city_coords[37];
    QPixmap pixmap;
    QPen redPen, yellowPen, greenPen;
};
#endif // MAINWINDOW_H
