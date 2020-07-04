#include "mainwindow.h"
#include "args.h"
#include <QApplication>
#include <QDateTime>
#include <QFile>
#include <QTextStream>

void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QString strType = "";
    switch (type) {
        case QtDebugMsg:
            strType = "Debug";
            break;
        case QtWarningMsg:
            strType = "Warning";
            break;
        case QtCriticalMsg:
            strType = "Critical";
            break;
        case QtFatalMsg:
            strType = "Fatal";
            break;
        case QtInfoMsg:
            strType = "Info";
            break;
    }
    QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString strMessage = QString("[%1][%2]%3").arg(strType).arg(strDateTime).arg(msg);
    QFile file("log.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream stream(&file);
    stream << strMessage << "\n";
    stream.flush();
    file.close();
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    qInstallMessageHandler(outputMessage);
    MainWindow w;
    w.show();
    return a.exec();
}
