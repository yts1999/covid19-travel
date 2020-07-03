#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setFixedSize(800, 900);
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::on_timeLimitCheck_stateChanged(int arg1) {

}
