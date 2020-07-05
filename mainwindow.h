#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "solver.h"
#include <QMainWindow>
#include <QPointF>
#include <QPixmap>
#include <QPen>
#include <QHash>
#include <QTimer>
#include <vector>

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
    void on_simButton_clicked();
    void on_continueSimButton_clicked();
    void next_step();

private:
    struct Path {
        QPixmap linemap;
        int stTime, edTime;
        std::vector<int> path;
    };
    Ui::MainWindow *ui;
    QPoint city_coords[37];
    QPixmap pixmap, trainIcon, airplaneIcon, coachIcon, waitIcon;
    QPen redPen, yellowPen, greenPen;
    Solver solver;
    QPen trainPen, airplanePen, coachPen;
    QHash<QString, Path> userPath;
    QString time2str(int t) const;
    QPixmap simLinemap;
    int simStTime, simEdTime;
    std::vector<int> simPath;
    std::vector<std::pair<int, int> > timePeriod;
    int simState, curTime, curPeriod;
    QString simUsr;
    QTimer simTimer;
};
#endif // MAINWINDOW_H
