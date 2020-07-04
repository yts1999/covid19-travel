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

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent), ui(new Ui::MainWindow), pixmap(":/map/map.jpg") {
    ui->setupUi(this);
    setFixedSize(800, 920);
    move((QApplication::desktop()->width() - this->width()) / 2, (QApplication::desktop()->height() - this->height()) / 2);
    city_coords[0] = QPointF(583, 267);
    city_coords[1] = QPointF(601, 278.5);
    city_coords[2] = QPointF(565, 311);
    city_coords[3] = QPointF(535, 315);
    city_coords[4] = QPointF(530, 258);
    city_coords[5] = QPointF(667, 232);
    city_coords[6] = QPointF(656, 275);
    city_coords[7] = QPointF(686, 174);
    city_coords[8] = QPointF(695, 149);
    city_coords[9] = QPointF(679, 405);
    city_coords[10] = QPointF(643, 402);
    city_coords[11] = QPointF(662, 425);
    city_coords[12] = QPointF(688, 437);
    city_coords[13] = QPointF(620, 412);
    city_coords[14] = QPointF(660, 497);
    city_coords[15] = QPointF(642, 530.5);
    city_coords[16] = QPointF(612, 453);
    city_coords[17] = QPointF(598, 332);
    city_coords[18] = QPointF(653, 321);
    city_coords[19] = QPointF(555, 360);
    city_coords[20] = QPointF(575, 427);
    city_coords[21] = QPointF(558, 469);
    city_coords[22] = QPointF(575, 561);
    city_coords[23] = QPointF(582, 570);
    city_coords[24] = QPointF(501, 578);
    city_coords[25] = QPointF(533, 618);
    city_coords[26] = QPointF(426, 438);
    city_coords[27] = QPointF(471, 506);
    city_coords[28] = QPointF(404, 534);
    city_coords[29] = QPointF(239, 443);
    city_coords[30] = QPointF(464, 455);
    city_coords[31] = QPointF(492, 368);
    city_coords[32] = QPointF(418, 343);
    city_coords[33] = QPointF(387, 335);
    city_coords[34] = QPointF(450, 311);
    city_coords[35] = QPointF(226, 197);
    city_coords[36] = QPointF(692, 505);
    pixmap = pixmap.scaled(ui->map->width(), ui->map->height(), Qt::IgnoreAspectRatio);
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
    for (int i = 0; i < city_num; i++) {
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
    for (int i = 0; i < city_num; i++) {
        ui->startInput->addItem(cities[i]);
        ui->endInput->addItem(cities[i]);
    }
}

MainWindow::~MainWindow() {
    delete ui;
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
}
