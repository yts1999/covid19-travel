#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "args.h"
#include <QPixmap>
#include <QPainter>
#include <QPen>
#include <QColor>
#include <iostream>
#include <QMessageBox>
#include <QApplication>
#include <QDesktopWidget>
#include <QRegExpValidator>
#include <QRegExp>
#include <QtDebug>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent), ui(new Ui::MainWindow), pixmap(":/map/map.jpg"), trainIcon(":/map/train.png"), airplaneIcon(":/map/airplane.png"), coachIcon(":/map/coach.png"), waitIcon(":/map/wait.png") {
    ui->setupUi(this);
    setFixedSize(800, 920);
    move((QApplication::desktop()->width() - this->width()) / 2, (QApplication::desktop()->height() - this->height()) / 2);
    ui->userInput->setValidator(new QRegExpValidator(QRegExp("^[A-Za-z0-9_]{0,15}$"), this));
    city_coords[0] = QPoint(583, 267);
    city_coords[1] = QPoint(601, 278);
    city_coords[2] = QPoint(565, 311);
    city_coords[3] = QPoint(535, 315);
    city_coords[4] = QPoint(530, 258);
    city_coords[5] = QPoint(667, 232);
    city_coords[6] = QPoint(656, 275);
    city_coords[7] = QPoint(686, 174);
    city_coords[8] = QPoint(695, 149);
    city_coords[9] = QPoint(679, 405);
    city_coords[10] = QPoint(643, 402);
    city_coords[11] = QPoint(662, 425);
    city_coords[12] = QPoint(688, 437);
    city_coords[13] = QPoint(620, 412);
    city_coords[14] = QPoint(660, 497);
    city_coords[15] = QPoint(642, 530);
    city_coords[16] = QPoint(612, 453);
    city_coords[17] = QPoint(598, 332);
    city_coords[18] = QPoint(653, 321);
    city_coords[19] = QPoint(555, 360);
    city_coords[20] = QPoint(575, 427);
    city_coords[21] = QPoint(558, 469);
    city_coords[22] = QPoint(575, 561);
    city_coords[23] = QPoint(582, 570);
    city_coords[24] = QPoint(501, 578);
    city_coords[25] = QPoint(533, 618);
    city_coords[26] = QPoint(426, 438);
    city_coords[27] = QPoint(471, 506);
    city_coords[28] = QPoint(404, 534);
    city_coords[29] = QPoint(239, 443);
    city_coords[30] = QPoint(464, 455);
    city_coords[31] = QPoint(492, 368);
    city_coords[32] = QPoint(418, 343);
    city_coords[33] = QPoint(387, 335);
    city_coords[34] = QPoint(450, 311);
    city_coords[35] = QPoint(226, 197);
    city_coords[36] = QPoint(692, 505);
    pixmap = pixmap.scaled(ui->map->width(), ui->map->height(), Qt::IgnoreAspectRatio);
    trainIcon = trainIcon.scaled(ui->stateIcon->width(), ui->stateIcon->height(), Qt::IgnoreAspectRatio);
    airplaneIcon = airplaneIcon.scaled(ui->stateIcon->width(), ui->stateIcon->height(), Qt::IgnoreAspectRatio);
    coachIcon = coachIcon.scaled(ui->stateIcon->width(), ui->stateIcon->height(), Qt::IgnoreAspectRatio);
    waitIcon = waitIcon.scaled(ui->stateIcon->width(), ui->stateIcon->height(), Qt::IgnoreAspectRatio);
    QPainter painter(&pixmap);
    redPen.setWidth(10);
    yellowPen.setWidth(10);
    greenPen.setWidth(10);
    redPen.setColor(QColor("crimson"));
    yellowPen.setColor(QColor("gold"));
    greenPen.setColor(QColor("limegreen"));
    redPen.setCapStyle(Qt::RoundCap);
    yellowPen.setCapStyle(Qt::RoundCap);
    greenPen.setCapStyle(Qt::RoundCap);
    painter.setPen(redPen);
    painter.drawPoint(700, 602);
    painter.setPen(yellowPen);
    painter.drawPoint(700, 622);
    painter.setPen(greenPen);
    painter.drawPoint(700, 642);
    for (int i = 0; i < CITY_NUM; i++) {
        switch (risk_level[i]) {
            case 2:
                painter.setPen(redPen);
                break;
            case 1:
                painter.setPen(yellowPen);
                break;
            case 0:
                painter.setPen(greenPen);
                break;
        }
        painter.drawPoint(city_coords[i]);
    }
    ui->map->setPixmap(pixmap);
    for (int i = 0; i < CITY_NUM; i++) {
        ui->startInput->addItem(cities[i]);
        ui->endInput->addItem(cities[i]);
    }
    trainPen.setWidth(2);
    trainPen.setColor("darkgreen");
    airplanePen.setWidth(2);
    airplanePen.setColor("navy");
    coachPen.setWidth(2);
    coachPen.setColor("grey");
    simState = 0;
    connect(&simTimer, SIGNAL(timeout()), this, SLOT(next_step()));
}

MainWindow::~MainWindow() {
    delete ui;
}

QString MainWindow::time2str(int t) const {
    QString strTime;
    if (t < 24)
        strTime = QString("%1时").arg(t);
    else
        if (t < 48)
            strTime = QString("次日%1时").arg(t - 24);
        else
            strTime = QString("第%1日%2时").arg(t / 24 + 1).arg(t % 24);
    return strTime;
}

void MainWindow::on_timeLimitCheck_stateChanged(int state) {
    switch (state) {
        case Qt::Checked:
            ui->dayInput->setEnabled(true);
            ui->dayLabel->setEnabled(true);
            ui->hourInput->setEnabled(true);
            ui->hourLabel->setEnabled(true);
            break;
        case Qt::Unchecked:
            ui->dayInput->setEnabled(false);
            ui->dayLabel->setEnabled(false);
            ui->hourInput->setEnabled(false);
            ui->hourLabel->setEnabled(false);
            break;
    }
}

void MainWindow::on_startButton_clicked() {
    if (simState == 1) {
        simTimer.stop();
        qInfo() << "用户" << simUsr << "暂停模拟";
        simState = 2;
        ui->continueSimButton->setEnabled(true);
    }
    QString usr = ui->userInput->text();
    if (usr == "") {
        QMessageBox::warning(this, "提示", "请输入用户名！");
        return;
    }
    int st = ui->startInput->currentIndex(), ed = ui->endInput->currentIndex();
    if (st == ed) {
        QMessageBox::warning(this, "提示", "出发城市和目的城市不能相同！");
        return;
    }
    int stTime = ui->stTimeInput->value();
    bool timeLimit = ui->timeLimitCheck->isChecked();
    std::pair<std::pair<double, int>, std::vector<int> > result;
    if (! timeLimit) {
        qInfo() << "接收到用户" << usr << "的线路规划请求。出发城市：" << cities[st] << "目的城市：" << cities[ed] << "出发时间：" << stTime;
        result = solver.solve(st, stTime, ed);
    }
    else {
        int timeLimit = ui->dayInput->value() * 24 + ui->hourInput->value();
        qInfo() << "接收到用户" << usr << "的限时线路规划请求。出发城市：" << cities[st] << "目的城市：" << cities[ed] << "出发时间：" << stTime << "时间限制：" << timeLimit;
        result = solver.solve(st, stTime, ed, timeLimit);
    }
    ui->resultBrowser->clear();
    if (result.first.second == -1) {
        ui->resultBrowser->append("无法到达目的城市！");
        qInfo() << "计算结束，无法到达目的城市";
        return;
    }
    ui->resultBrowser->append(QString("累计风险值：%1    累计用时：%2小时    到达时间：%3").arg(result.first.first).arg(result.first.second - stTime).arg(time2str(result.first.second)));
    qInfo() << "计算结束，累计风险值：" << result.first.first << "累计用时：" << result.first.second - stTime << " 到达时间：" << time2str(result.first.second);
    userPath[usr].linemap = pixmap.copy();
    userPath[usr].stTime = stTime;
    userPath[usr].edTime = result.first.second;
    userPath[usr].path = result.second;
    for (int curTime = stTime, i = 0; i < result.second.size(); i++) {
        int nxtTime;
        if (curTime % 24 != timetable[result.second[i]].stTime) {
            if (curTime % 24 < timetable[result.second[i]].stTime)
                nxtTime = curTime + timetable[result.second[i]].stTime - curTime % 24;
            else
                nxtTime = curTime + timetable[result.second[i]].stTime - curTime % 24 + 24;
            ui->resultBrowser->append(QString("%1 - %2  在%3等待").arg(time2str(curTime)).arg(time2str(nxtTime)).arg(cities[timetable[result.second[i]].st]));
            curTime = nxtTime;
        }
        nxtTime = curTime + timetable[result.second[i]].dur;
        QPainter painter(&userPath[usr].linemap);
        switch (timetable[result.second[i]].type) {
            case TRAIN:
                ui->resultBrowser->append(QString("%1 - %2 火车 %3→%4").arg(time2str(curTime)).arg(time2str(nxtTime)).arg(cities[timetable[result.second[i]].st]).arg(cities[timetable[result.second[i]].ed]));
                painter.setPen(trainPen);
                break;
            case AIRPLANE:
                ui->resultBrowser->append(QString("%1 - %2 飞机 %3→%4").arg(time2str(curTime)).arg(time2str(nxtTime)).arg(cities[timetable[result.second[i]].st]).arg(cities[timetable[result.second[i]].ed]));
                painter.setPen(airplanePen);
                break;
            case COACH:
                ui->resultBrowser->append(QString("%1 - %2 汽车 %3→%4").arg(time2str(curTime)).arg(time2str(nxtTime)).arg(cities[timetable[result.second[i]].st]).arg(cities[timetable[result.second[i]].ed]));
                painter.setPen(coachPen);
                break;
        }
        painter.drawLine(city_coords[timetable[result.second[i]].st], city_coords[timetable[result.second[i]].ed]);
        curTime = nxtTime;
    }
    ui->map->setPixmap(userPath[usr].linemap);
    ui->stateIcon->clear();
    ui->resultBrowser->verticalScrollBar()->setValue(0);
}

void MainWindow::on_simButton_clicked() {
    QString usr = ui->simUserInput->text();
    if (usr == "") {
        QMessageBox::warning(this, "提示", "请输入用户名！");
        return;
    }
    if (! userPath.contains(usr)) {
        QMessageBox::warning(this, "提示", "该用户无线路规划记录！");
        return;
    }
    qInfo() << "用户" << usr << "开始模拟";
    simState = 1;
    ui->continueSimButton->setEnabled(false);
    simUsr = usr;
    simLinemap = userPath[usr].linemap;
    simStTime = userPath[usr].stTime;
    simEdTime = userPath[usr].edTime;
    simPath = userPath[usr].path;
    curTime = simStTime;
    curPeriod = 0;
    ui->simResultBrowser->clear();
    ui->simUserLabel->setText(simUsr);
    ui->timeLabel->setText(time2str(curTime));
    ui->map->setPixmap(simLinemap);
    timePeriod.resize(simPath.size());
    for (int curTime = simStTime, i = 0; i < simPath.size(); i++) {
        int nxtTime;
        if (curTime % 24 != timetable[simPath[i]].stTime) {
            if (curTime % 24 < timetable[simPath[i]].stTime)
                nxtTime = curTime + timetable[simPath[i]].stTime - curTime % 24;
            else
                nxtTime = curTime + timetable[simPath[i]].stTime - curTime % 24 + 24;
            curTime = nxtTime;
        }
        nxtTime = curTime + timetable[simPath[i]].dur;
        timePeriod[i] = std::make_pair(curTime, nxtTime);
        curTime = nxtTime;
    }
    if (curTime < timePeriod[curPeriod].first) {
        ui->stateIcon->setPixmap(waitIcon);
        ui->simResultBrowser->append(QString("%1 在%2等待").arg(time2str(curTime)).arg(cities[timetable[simPath[curPeriod]].st]));
        qInfo() << "时间：" << time2str(curTime) << "用户在" << cities[timetable[simPath[curPeriod]].st] << "等待";
    }
    else
        switch (timetable[simPath[curPeriod]].type) {
            case TRAIN:
                ui->stateIcon->setPixmap(trainIcon);
                ui->simResultBrowser->append(QString("%1 乘坐%2到%3的火车").arg(time2str(curTime)).arg(cities[timetable[simPath[curPeriod]].st]).arg(cities[timetable[simPath[curPeriod]].ed]));
                qInfo() << "时间：" << time2str(curTime) << "用户乘坐" << cities[timetable[simPath[curPeriod]].st] << "到" << cities[timetable[simPath[curPeriod]].ed] << "的火车";
                break;
            case AIRPLANE:
                ui->stateIcon->setPixmap(airplaneIcon);
                ui->simResultBrowser->append(QString("%1 乘坐%2到%3的飞机").arg(time2str(curTime)).arg(cities[timetable[simPath[curPeriod]].st]).arg(cities[timetable[simPath[curPeriod]].ed]));
                qInfo() << "时间：" << time2str(curTime) << "用户乘坐" << cities[timetable[simPath[curPeriod]].st] << "到" << cities[timetable[simPath[curPeriod]].ed] << "的飞机";
                break;
            case COACH:
                ui->stateIcon->setPixmap(coachIcon);
                ui->simResultBrowser->append(QString("%1 乘坐%2到%3的汽车").arg(time2str(curTime)).arg(cities[timetable[simPath[curPeriod]].st]).arg(cities[timetable[simPath[curPeriod]].ed]));
                qInfo() << "时间：" << time2str(curTime) << "用户乘坐" << cities[timetable[simPath[curPeriod]].st] << "到" << cities[timetable[simPath[curPeriod]].ed] << "的汽车";
                break;
        }
    ui->stateIcon->setGeometry(city_coords[timetable[simPath[curPeriod]].st].x() - ui->stateIcon->width() / 2, city_coords[timetable[simPath[curPeriod]].st].y() - ui->stateIcon->height() / 2, ui->stateIcon->width(), ui->stateIcon->height());
    simTimer.start(10000);
}

void MainWindow::on_continueSimButton_clicked() {
    qInfo() << "用户" << simUsr << "继续模拟";
    simState = 1;
    ui->continueSimButton->setEnabled(false);
    ui->simUserLabel->setText(simUsr);
    ui->timeLabel->setText(time2str(curTime));
    ui->map->setPixmap(simLinemap);
    if (curTime < timePeriod[curPeriod].first) {
        ui->stateIcon->setGeometry(city_coords[timetable[simPath[curPeriod]].st].x() - ui->stateIcon->width() / 2, city_coords[timetable[simPath[curPeriod]].st].y() - ui->stateIcon->height() / 2, ui->stateIcon->width(), ui->stateIcon->height());
        ui->stateIcon->setPixmap(waitIcon);
    }
    else {
        int x = ((timePeriod[curPeriod].second - curTime) * city_coords[timetable[simPath[curPeriod]].st].x() + (curTime - timePeriod[curPeriod].first) * city_coords[timetable[simPath[curPeriod]].ed].x()) / (timePeriod[curPeriod].second - timePeriod[curPeriod].first);
        int y = ((timePeriod[curPeriod].second - curTime) * city_coords[timetable[simPath[curPeriod]].st].y() + (curTime - timePeriod[curPeriod].first) * city_coords[timetable[simPath[curPeriod]].ed].y()) / (timePeriod[curPeriod].second - timePeriod[curPeriod].first);
        ui->stateIcon->setGeometry(x - ui->stateIcon->width() / 2, y - ui->stateIcon->height() / 2, ui->stateIcon->width(), ui->stateIcon->height());
        switch (timetable[simPath[curPeriod]].type) {
            case TRAIN:
                ui->stateIcon->setPixmap(trainIcon);
                break;
            case AIRPLANE:
                ui->stateIcon->setPixmap(airplaneIcon);
                break;
            case COACH:
                ui->stateIcon->setPixmap(coachIcon);
                break;
        }
    }
    simTimer.start(10000);
}

void MainWindow::next_step() {
    curTime++;
    ui->timeLabel->setText(time2str(curTime));
    if (curTime == timePeriod[curPeriod].first)
        switch (timetable[simPath[curPeriod]].type) {
            case TRAIN:
                ui->simResultBrowser->append(QString("%1 乘坐%2到%3的火车").arg(time2str(curTime)).arg(cities[timetable[simPath[curPeriod]].st]).arg(cities[timetable[simPath[curPeriod]].ed]));
                qInfo() << "时间：" << time2str(curTime) << "用户乘坐" << cities[timetable[simPath[curPeriod]].st] << "到" << cities[timetable[simPath[curPeriod]].ed] << "的火车";
                break;
            case AIRPLANE:
                ui->simResultBrowser->append(QString("%1 乘坐%2到%3的飞机").arg(time2str(curTime)).arg(cities[timetable[simPath[curPeriod]].st]).arg(cities[timetable[simPath[curPeriod]].ed]));
                qInfo() << "时间：" << time2str(curTime) << "用户乘坐" << cities[timetable[simPath[curPeriod]].st] << "到" << cities[timetable[simPath[curPeriod]].ed] << "的飞机";
                break;
            case COACH:
                ui->simResultBrowser->append(QString("%1 乘坐%2到%3的汽车").arg(time2str(curTime)).arg(cities[timetable[simPath[curPeriod]].st]).arg(cities[timetable[simPath[curPeriod]].ed]));
                qInfo() << "时间：" << time2str(curTime) << "用户乘坐" << cities[timetable[simPath[curPeriod]].st] << "到" << cities[timetable[simPath[curPeriod]].ed] << "的汽车";
                break;
        }
    else
        if (curTime == timePeriod[curPeriod].second) {
            ui->simResultBrowser->append(QString("%1 到达%2").arg(time2str(curTime)).arg(cities[timetable[simPath[curPeriod]].ed]));
            qInfo() << "时间：" << time2str(curTime) << "用户到达" << cities[timetable[simPath[curPeriod]].ed];
            curPeriod++;
            if (curPeriod == timePeriod.size()) {
                simTimer.stop();
                ui->simResultBrowser->append(QString("到达目的地，模拟结束"));
                qInfo() << "用户到达目的地，模拟结束";
                simState = 0;
            }
            else
                if (curTime < timePeriod[curPeriod].first) {
                    ui->simResultBrowser->append(QString("%1 在%2等待").arg(time2str(curTime)).arg(cities[timetable[simPath[curPeriod]].st]));
                    qInfo() << "时间：" << time2str(curTime) << "用户在" << cities[timetable[simPath[curPeriod]].st] << "等待";
                }
                else
                    switch (timetable[simPath[curPeriod]].type) {
                        case TRAIN:
                            ui->simResultBrowser->append(QString("%1 乘坐%2到%3的火车").arg(time2str(curTime)).arg(cities[timetable[simPath[curPeriod]].st]).arg(cities[timetable[simPath[curPeriod]].ed]));
                            qInfo() << "时间：" << time2str(curTime) << "用户乘坐" << cities[timetable[simPath[curPeriod]].st] << "到" << cities[timetable[simPath[curPeriod]].ed] << "的火车";
                            break;
                        case AIRPLANE:
                            ui->simResultBrowser->append(QString("%1 乘坐%2到%3的飞机").arg(time2str(curTime)).arg(cities[timetable[simPath[curPeriod]].st]).arg(cities[timetable[simPath[curPeriod]].ed]));
                            qInfo() << "时间：" << time2str(curTime) << "用户乘坐" << cities[timetable[simPath[curPeriod]].st] << "到" << cities[timetable[simPath[curPeriod]].ed] << "的飞机";
                            break;
                        case COACH:
                            ui->simResultBrowser->append(QString("%1 乘坐%2到%3的汽车").arg(time2str(curTime)).arg(cities[timetable[simPath[curPeriod]].st]).arg(cities[timetable[simPath[curPeriod]].ed]));
                            qInfo() << "时间：" << time2str(curTime) << "用户乘坐" << cities[timetable[simPath[curPeriod]].st] << "到" << cities[timetable[simPath[curPeriod]].ed] << "的汽车";
                            break;
                    }
        }
    ui->stateIcon->clear();
    if (curPeriod == timePeriod.size()) {
        ui->stateIcon->setGeometry(city_coords[timetable[simPath.back()].ed].x() - ui->stateIcon->width() / 2, city_coords[timetable[simPath.back()].ed].y() - ui->stateIcon->height() / 2, ui->stateIcon->width(), ui->stateIcon->height());
        switch (timetable[simPath.back()].type) {
            case TRAIN:
                ui->stateIcon->setPixmap(trainIcon);
                break;
            case AIRPLANE:
                ui->stateIcon->setPixmap(airplaneIcon);
                break;
            case COACH:
                ui->stateIcon->setPixmap(coachIcon);
                break;
        }
    }
    else
        if (curTime < timePeriod[curPeriod].first) {
            ui->stateIcon->setGeometry(city_coords[timetable[simPath[curPeriod]].st].x() - ui->stateIcon->width() / 2, city_coords[timetable[simPath[curPeriod]].st].y() - ui->stateIcon->height() / 2, ui->stateIcon->width(), ui->stateIcon->height());
            ui->stateIcon->setPixmap(waitIcon);
        }
        else {
            int x = ((timePeriod[curPeriod].second - curTime) * city_coords[timetable[simPath[curPeriod]].st].x() + (curTime - timePeriod[curPeriod].first) * city_coords[timetable[simPath[curPeriod]].ed].x()) / (timePeriod[curPeriod].second - timePeriod[curPeriod].first);
            int y = ((timePeriod[curPeriod].second - curTime) * city_coords[timetable[simPath[curPeriod]].st].y() + (curTime - timePeriod[curPeriod].first) * city_coords[timetable[simPath[curPeriod]].ed].y()) / (timePeriod[curPeriod].second - timePeriod[curPeriod].first);
            ui->stateIcon->setGeometry(x - ui->stateIcon->width() / 2, y - ui->stateIcon->height() / 2, ui->stateIcon->width(), ui->stateIcon->height());
            switch (timetable[simPath[curPeriod]].type) {
                case TRAIN:
                    ui->stateIcon->setPixmap(trainIcon);
                    break;
                case AIRPLANE:
                    ui->stateIcon->setPixmap(airplaneIcon);
                    break;
                case COACH:
                    ui->stateIcon->setPixmap(coachIcon);
                    break;
            }
        }
}
